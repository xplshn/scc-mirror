
BEGIN		{
		FS = "\t"
		printf "#include \"../../../inc/scc.h\"\n"\
		       "#include \"../../as.h\"\n"\
		       "#include \"../" family "/proc.h\"\n"
		nop = 0; nvar = 0

		rules = "target/" family "/rules.dat" 
		while (getline < rules > 0) {
			regex[++nregs] = $1
			value[nregs] = $2
		}
		close "target/rules.awk"
}
		{sub(/#.*/,"")}

$6 !~ cpu	{next}

/^$/		{next}

		{
		if (opstart[$1] == 0) {
			opstart[$1] = nvar
			opnames[nop++] = $1
		}
		opcount[$1]++
		opargs[nvar] = $2
		opsize[nvar] = $3
		opbytes[nvar] = ($4 == "none") ? "" : $4
		opformat[nvar++] = $5
		formats[$5] = 1
}

END		{
		for (i in formats)
			printf "Format %s;\n", i

		printf "int nr_ins = %d;\n\n", nop
		print "struct ins instab[] = {"
		for (i = 0; i < nop; i++) {
			n = opnames[i]
			start = opstart[n]
			end = start + opcount[n]
			printf "\t{.str = \"%s\", .begin = %d, .end = %d},\n",
			       n, start, end | "sort"
		}
		close("sort")
		printf "};\n\n"

		print "struct op optab[] = {"
		for (i = 0; i < nvar; i++) {
			printf "\t/* %d */\n", i
			printf "\t{\n" \
			       "\t\t.size = %d,\n"\
			       "\t\t.format = %s,\n",
			       opsize[i], opformat[i]

			if (opbytes[i] != "")
				printf "\t\t.bytes = (unsigned char []) {%s},\n",
				        opbytes[i]

			a = str2args(opargs[i])
			if (a != "")
				printf "\t\t.args = (unsigned char []) {%s}\n", a

			print "\t},"
		}
		print "};"
}

function str2args(s, args, i, j, out, n, found)
{
	n = split(s, args, /,/)
	if (n == 0 || args[1] == "none")
		return ""
	for (i = 1; i <= n; i++) {
		a = args[i]
		found = 0
		for (j = 1; j <= nregs; j++) {
			if (match(a, regex[j])) {
				out = out value[j]
				found = 1
				break
			}
		}

		if (!found) {
			print FILENAME ":" NR ":" \
			      $0 ":wrong arg", a > "/dev/stderr"
			exit 1
		}

		a = substr(a, RLENGTH+1)
		if (a ~ /^\+$/) {
			return out "|AREP"
		} else if (a ~ /^\?$/) {
			return out "|AOPT"
		} else if (a != "") {
			print FILENAME ":" NR ":" \
			      $0 ": trailing chars: ", a > "/dev/stderr"
			exit 1
		}
		out = out ","
	}
	out = out "0"

	return out
}
