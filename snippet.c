// Snippet that uses strsep instead of strtok
// WARNING: Might contain bugs
    /* char *parsed; */
    /* while ((parsed=strsep(&line, separator)) != NULL) { */
    /*     if (strcmp(parsed, "") == 0) { */
    /*         break; */
    /*     } */

    /*     if (word_count % block == 0) { */
    /*         inp_ptr = expand_argv(inp_ptr); */
    /*         if (!inp_ptr) */
    /*             return NULL; */
    /*     } */

    /*     inp_ptr->argv[word_count++] = parsed; */
    /* } */

    /* if (word_count == 0) */
    /*     return NULL; */


