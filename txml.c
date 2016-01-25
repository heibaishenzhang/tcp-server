#include <stdio.h>
#include <string.h>

const char* sample_xml="<term_update_info>" \
	"<terminalID>hello</terminalID>" \
	"<vpnGateID>I'm here</vpnGateID>" \
	"<terminalType></terminalType>" \
	"<terminalOS></terminalOS>" \
    "<terminalHD></terminalHD>" \
	"<terminalCltVer></terminalCltVer>" \
	"<userCert></userCert>" \
	"<userSID></userSID>" \
	"<updateTime></updateTime>" \
	"<updateStatus></updateStatus>" \
	"<res1></res1>" \
	"<res1></res2>" \
"</term_update_info>";

int main(int argc, char* argv[])
{
    char *p = NULL;
    char *q = NULL;
    char buf[20] = {'\0'};
    p = strstr(sample_xml, "terminalID");    
    q = p + strlen("terminalID>");
    printf("p=%s\n", p);
    printf("q=%s\n", q);
    sscanf(buf, "%s</terminalID>", q);
    printf("buf=%s\n");
    p = NULL;
    p = strstr(sample_xml, "vpnGateID");
    printf("p=%s\n", p);
}
