#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int
main(
  int    argc,
  char** argv)
{
  int exit_code = EXIT_FAILURE;

  if (argc >= 3)
  {
    FILE* fp = fopen(argv[1], "rb+");
    if(fp)
    {
      size_t sz = 0;
      (void) fseek(fp, 0L, SEEK_END);
      sz = (size_t)ftell(fp);
      if(sz)
      {
        char* buf = malloc(sz);
        if(buf)
        {
          int byte_drift = 0;
          int tokens = argc - 1;
          rewind(fp);
          (void) fread(buf, 1, sz, fp);
          for(int i = 0; i < (long)sz; ++i)
          {
            for(int j = 0; j < tokens; ++j)
            {
              int len = (int)strlen(argv[1 + j]);
              int insert_index = i + len + byte_drift;
              for(int k = 0; k < len; ++k)
              {
                int stable_index = i + k;
                if(i + len >= (long)sz)
                {
                  break;
                }
                if(buf[stable_index] != argv[1 + j][k])
                {
                  break;
                }
                if(k < len - 1)
                {
                  continue;
                }
                else if(insert_index < (long)(sz + byte_drift))
                {
                  (void) fseek(fp, (long)insert_index, SEEK_SET);
                  (void) fwrite("\n", 1, 1, fp);
                  
                  if(!isspace(buf[stable_index + 1]))
                  {
                    (void) fwrite(&buf[stable_index + 1], 1, sz - stable_index - 1 , fp);
                    ++byte_drift;
                  }
                  buf[stable_index + 1] = '\n';
                }
                break;
              }
            }
          }
          exit_code = EXIT_SUCCESS;
        }
      }
      (void) fclose(fp);
    }
  }
  return exit_code;
}