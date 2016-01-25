#include <iconv.h>    
#include <stdio.h>
#include <libxml/parser.h>    
#include <libxml/xmlmemory.h> 

const char* sample_xml ="<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"yes\"?>" \
	"<term_update_info>" \
	"<terminalID>default</terminalID>" \
	"<vpnGateID>default</vpnGateID>" \
	"<terminalType>1</terminalType>" \
	"<terminalTypeName>default</terminalTypeName>" \
	"<terminalOS>Microsoft Windows 7 .</terminalOS>" \
	"<terminalHD>HD_SN:            S2A34XG2</terminalHD> " \
	"<terminalCltVer>3.6.0.5558</terminalCltVer>" \
	"<userCert>default</userCert>" \
	"<userSID>default</userSID>" \
	"<userDepartment>default</userDepartment>" \
	"<userOrganization>default</userOrganization>" \
	"<updateTime>2016-01-25 09:53:08</updateTime>" \
	"<updateStatus>0</updateStatus>" \
	"<res1>default</res1>" \
	"<res2>default</res2>" \
	"</term_update_info>";

#define FIELD_LEN 1024    

int conv_charset(const char *dest, const char *src, char *input, size_t ilen, char *output, size_t olen) {    
	iconv_t conv = iconv_open(dest, src);    
	if ( conv == (iconv_t) -1 )    
		return -1;    
	memset(output, 0, olen);    
	if ( iconv(conv, &input, &ilen, &output, &olen) )    
		return -1;    
	iconv_close(conv);    
	return 0;    
}    

int main(int argc, char* argv[])
{
	xmlDocPtr pdoc = xmlParseMemory(sample_xml, strlen(sample_xml));
	if(NULL == pdoc)
		return -1;
	xmlNodePtr root = xmlDocGetRootElement(pdoc);    
	xmlNodePtr curr = root;  

	while ( xmlStrcmp(curr->name, (const xmlChar *) "term_update_info") )    
		curr = curr->xmlChildrenNode;    
	for ( curr = curr->xmlChildrenNode; curr; curr = curr->next ) {    
		xmlNodePtr data;    
		for ( data = curr->xmlChildrenNode; data; data = data->next ) {    
			char ifield[FIELD_LEN];    
			char ofield[FIELD_LEN];    
			strcpy(ifield, xmlNodeGetContent(data));    
			//if ( conv_charset("GBK", "UTF-8", ifield, strlen(ifield), ofield, FIELD_LEN) )    
				printf("%s\t%s\n", data->name, ifield);    
			//else    
			//	printf("%s\t%s\n", data->name, ofield);    
		}    
		printf("\n");    
	}    
	xmlFreeDoc(pdoc);
}
