#define _CRT_DISABLE_PERFCRIT_LOCKS
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
      (void) fseek(fp, (long)0, SEEK_END);
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
          (void) setvbuf(fp, NULL, _IOFBF, BUFSIZ);
          for(int i = 0; (size_t)i < sz; ++i)
          {
            for(int j = 0; j < tokens; ++j)
            {
              int len = (int)strlen(argv[1 + j]);
              int insert_index = i + len + byte_drift;
              for(int k = 0; k < len; ++k)
              {
                int stable_index = i + k;
                int next_index = stable_index + 1;
                if(sz <= (size_t)(i + len))
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
                if(insert_index < (long)(sz + byte_drift))
                {
                  if(buf[next_index] != '\n')
                  {
                    (void) fseek(fp, (long)insert_index, SEEK_SET);
                    (void) fwrite("\n", 1, 1, fp);
                    
                    if(!isspace(buf[next_index]))
                    {
                      (void) fwrite(&buf[next_index], 1, sz - stable_index - 1, fp);
                      ++byte_drift;
                    }
                    buf[next_index] = '\n';
                  }
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