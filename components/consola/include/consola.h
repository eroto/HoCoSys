#ifndef CONSOLA_H
#define CONSOLA_H

static const char* TAG = "consola";

// Command handler for user input
static int cmd_echo(int argc, char** argv);
static int restart(int argc, char **argv);
static int cmd_temp(int argc, char** argv);

#endif /*CONSOLA_H*/
