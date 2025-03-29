#include <scc/scc.h>

#include "../cc2.h"
#include "arch.h"

static void
deljmp(void)
{
	if (!pc)
		return;

	if (pc->op == ASJP || pc->op == ASJR) {
		if (pc->from1.u.sym == pc->next->label)
			delcode();
	}
}

void
peephole(void)
{
	for (pc = prog; pc; pc = pc->next) {
		deljmp();
	}
}
