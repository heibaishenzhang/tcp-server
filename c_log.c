void log(const char *fmt, ...)
{
    va_list ap;
    va_start(ap, fmt);
    
    time_t t;
    char ct[2048]; 
    ct[0] = '\0';
    char path_tmp[1024];
    struct tm tm = {0};

    FILE *fp;
    
    sprintf(path_tmp, "%s%s", log_path, "/0000");
    
    if (time(&t) > 0)
    {
       localtime_r(&t, &tm);
       (void) strftime (ct, sizeof(ct),"%OY-%Om-%Od %OH:%OM:%S ", &tm);
    }
    
    sprintf(ct+strlen(ct), "%s ", sysip);
    sprintf(ct+strlen(ct), "%s ", (char *)getprogname());
    vsprintf(ct+strlen(ct), fmt, ap);
    strcat(ct, "\n");
    
    fp = fopen(path_tmp, "a+");
    if(!fp)
    {
      fputs(ct, stderr);
    }
    else
    {
      fputs(ct, fp);
      fclose(fp);
    }
    va_end(ap);
}