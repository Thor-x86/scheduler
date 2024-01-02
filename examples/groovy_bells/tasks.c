//! [Groovy Bells Tasks Source Code]

#include <stdio.h>

void
clear_terminal_task ()
{
	fputs ("|       |       |       |\r", stdout);
}

void
bell_1000ms_task ()
{
	fputs ("\a| XXXXX |       |       |\r", stdout);
}

void
bell_1500ms_task ()
{
	fputs ("\a|       | XXXXX |       |\r", stdout);
}

void
bell_2250ms_task ()
{
	fputs ("\a|       |       | XXXXX |\r", stdout);
}

//! [Groovy Bells Tasks Source Code]