/^enum nerrors \{/     {print "char *errlist[] = {"; inhome = 1}

inhome && /E[A-Z]*, /  {sub(/,/, "", $1)
                        printf("\t[%s] = ", $1)
                        $1 = $2 = $NF = ""
                        sub(/^ */, "")
                        sub(/ *$/, "")
                        printf("\"%s\",\n", $0)}
inhome && /^}/          {print "};" ; inhome = 0}
