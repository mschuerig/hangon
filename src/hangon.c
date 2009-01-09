
#include <stdio.h>
#include <libintl.h>
#include <locale.h>

#include "config.h"

#define _(msgid) gettext(msgid)
#define N_(msgid) msgid

int
main(int argc, char *argv[])
{
  setlocale(LC_ALL, "");
  bindtextdomain(PACKAGE, LOCALEDIR);
  textdomain(PACKAGE);

  printf(_("Hello, World!\n"));
}
