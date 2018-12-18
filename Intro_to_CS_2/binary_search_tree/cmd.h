#ifndef _CMD_H
#define _CMD_H

/* ======================================= */
/* === command datatype and operations === */
/* ======================================= */

enum cmd_name {
  QUIT, STATS, READ, PREFIX, LOOKUP, HELP
};

typedef struct {
  enum cmd_name name;
  char* arg; /* optional argument */
} cmd;

/* === command constructors === */

/* cmd_new: make a new heap-allocated command with arg set to NULL */
cmd* cmd_new(enum cmd_name name);

/* cmd_read: make a new heap-allocated READ command */
/* note: copy filename (deep copy) */
cmd* cmd_read(const char* filename);

/* cmd_char: make a new heap-allocated PREFIX command */
/* note: copy prefix (deep copy) */
cmd* cmd_prefix(const char* prefix);

/* cmd_lookup: make a new heap-allocated LOOKUP command */
/* note: copy cnet (deep copy) */
cmd* cmd_lookup(const char* cnet);

/* === command free === */

/* cmd_free: free command struct and its string, if there is one */
void cmd_free(cmd* c);

/* === command display (for debugging, logging) === */

/* cmd_name_tos: return string constant for command name */
/* note: for debugging only; not currently used in shell */
char* cmd_name_tos(cmd* c);

/* cmd_show: print command to FILE* (typically stderr) */
/* note: for debugging only; not currently used in shell */
void cmd_show(FILE *f, cmd* c);

/* === command parsing === */

/* cmd_from_string: build a cmd (as defined above) from a string;
 * return NULL if the parse fails
 */
cmd* cmd_from_string(const char* s);

#endif /* _CMD_H */
