#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

#include <libxml/parser.h>
#include <libxml/tree.h>

#include "trace.h"

const char* sample_xml="<?xml version=\"1.0\" encoding=\"utf-8\"?>" \
"<term_update_info> <userCert>hhh</userCert> <userSID>旧呢就787987923429349242</userSID> <updateTime>2016-01-20 17:26:43</updateTime> <terminalCltVer>3.2.0</terminalCltVer> <terminalType>2</terminalType> <updateStatus>1</updateStatus> <terminalTypeName>SM-N9002</terminalTypeName> <terminalID>357555051008870</terminalID> <terminalHD>357555051008870</terminalHD> <CERTCARDOU>缺省</CERTCARDOU> <CERTCARDO>缺省</CERTCARDO> <terminalOS>Android 4.3</terminalOS> <res2>res2</res2> <res1>res1</res1> <vpnGateID>server</vpnGateID> </term_update_info>";


#define FIELD_LEN 128  

#if 0
int parse_xml(const char* buff, char* out)
{
    //parse_xml(sample_xml);        
    mxml_node_t *tree = NULL, *node = NULL;

    mxml_node_t *terminalid = NULL;
    mxml_node_t *terminalHD = NULL;
    mxml_node_t *userSID = NULL;
    mxml_node_t *userCert = NULL;
    mxml_node_t *updateTime = NULL;
    mxml_node_t *terminalCltVer = NULL;
    mxml_node_t *terminalType = NULL;
    mxml_node_t *updateStatus = NULL;
    mxml_node_t *terminalTypeName = NULL;
    mxml_node_t *terminalOS = NULL;
    mxml_node_t *vpnGateID = NULL;
    mxml_node_t *res1 = NULL;
    mxml_node_t *res2 = NULL;

    tree = mxmlLoadString(NULL, buff, MXML_TEXT_CALLBACK);
    node = mxmlFindElement(tree, tree, "term_update_info", NULL, NULL, MXML_DESCEND);
    
    terminalid = mxmlFindElement(node, tree, "terminalID", NULL, NULL, MXML_DESCEND);
    printf("terminalid=%s\n", terminalid->child->value.text.string); 

    userCert = mxmlFindElement(node, tree, "userCert", NULL, NULL, MXML_DESCEND);
    if(strlen(userCert->child->value.text.string) > 0)
	    printf("userCert=%s\n", userCert->child->value.text.string); 

    userSID = mxmlFindElement(node, tree, "userSID", NULL, NULL, MXML_DESCEND);
    printf("userid=%s\n", userSID->child->value.text.string); 

    updateTime = mxmlFindElement(node, tree, "updateTime", NULL, NULL, MXML_DESCEND);
    printf("updateTime=%s\n", updateTime->child->value.text.string); 

    terminalCltVer = mxmlFindElement(node, tree, "terminalCltVer", NULL, NULL, MXML_DESCEND);
    printf("terminalCltVer=%s\n", terminalCltVer->child->value.text.string); 

    terminalType = mxmlFindElement(node, tree, "terminalType", NULL, NULL, MXML_DESCEND);
    printf("terminalType=%s\n", terminalType->child->value.text.string); 

    updateStatus = mxmlFindElement(node, tree, "updateStatus", NULL, NULL, MXML_DESCEND);
    printf("updateStatus=%s\n", updateStatus->child->value.text.string); 

    terminalTypeName = mxmlFindElement(node, tree, "terminalTypeName", NULL, NULL, MXML_DESCEND);
    printf("terminalTypeName=%s\n", terminalTypeName->child->value.text.string); 

    terminalOS = mxmlFindElement(node, tree, "terminalOS", NULL, NULL, MXML_DESCEND);
    printf("terminalOS=%s\n", terminalOS->child->value.text.string); 

    terminalHD = mxmlFindElement(node, tree, "terminalHD", NULL, NULL, MXML_DESCEND);
    printf("terminalHD=%s\n", terminalHD->child->value.text.string); 

    vpnGateID = mxmlFindElement(node, tree, "vpnGateID", NULL, NULL, MXML_DESCEND);
    printf("vpnGateID=%s\n", vpnGateID->child->value.text.string); 

    res1 = mxmlFindElement(node, tree, "res1", NULL, NULL, MXML_DESCEND);
    printf("res1=%s\n", res1->child->value.text.string); 

    res2 = mxmlFindElement(node, tree, "res2", NULL, NULL, MXML_DESCEND);
    printf("res2=%s\n", res2->child->value.text.string); 

    snprintf(out, 4096, "INSERT INTO T_term_update_info(terminalID, userCert, userSID, updateTime, terminalCltVer, terminalType, updateStatus, terminalTypeName, terminalOS, terminalHD, vpnGateID, res1, res2) values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', '%s', '%s')",
		    terminalid->child->value.text.string,
		    userCert->child->value.text.string,
		    userSID->child->value.text.string,
		    updateTime->child->value.text.string,
		    terminalCltVer->child->value.text.string,
		    terminalType->child->value.text.string,
		    updateStatus->child->value.text.string, 
		    terminalTypeName->child->value.text.string,
		    terminalOS->child->value.text.string,
		    terminalHD->child->value.text.string,
		    vpnGateID->child->value.text.string, 
		    res1->child->value.text.string,
		    res2->child->value.text.string);
    TRACELOG("out=%s\n", out);

    mxmlDelete(tree);
    return 0;
}
#endif 

int conv_charset(const char *dest, const char *src, char *input, size_t ilen, char *output, size_t olen)
{    
	iconv_t conv = iconv_open(dest, src);    
	if ( conv == (iconv_t) -1 )    
		return -1;    
	memset(output, 0, olen);    
	if ( iconv(conv, &input, &ilen, &output, &olen) )    
		return -1;    
	iconv_close(conv);    
	return 0;    
}    

int parse_xml(const char* xml, char* out)
{
	char terminalID[FIELD_LEN] = {'\0'};
	char terminalHD[FIELD_LEN] = {'\0'};
	char userSID[FIELD_LEN] = {'\0'};
	char userCert[FIELD_LEN] = {'\0'};
	char updateTime[FIELD_LEN] = {'\0'};
	char terminalCltVer[FIELD_LEN] = {'\0'};
	char terminalType[FIELD_LEN] = {'\0'};
	char updateStatus[FIELD_LEN] = {'\0'};
	char terminalTypeName[FIELD_LEN] = {'\0'};
	char terminalOS[FIELD_LEN] = {'\0'};
	char vpnGateID[FIELD_LEN] = {'\0'};
	char res1[FIELD_LEN] = {'\0'};
	char res2[FIELD_LEN] = {'\0'};

	xmlDocPtr pdoc = xmlParseMemory(xml, strlen(xml));
	if(NULL == pdoc)
		return -1;
	xmlNodePtr root = xmlDocGetRootElement(pdoc);    
	xmlNodePtr curr = root;  

	while ( xmlStrcmp(curr->name, (const xmlChar *) "term_update_info") )    
		curr = curr->xmlChildrenNode;    
	for ( curr = curr->xmlChildrenNode; curr; curr = curr->next ) {    
		xmlNodePtr data;    
		for ( data = curr->xmlChildrenNode; data; data = data->next ) {    
			if(!xmlStrcmp(curr->name, (const xmlChar *)"terminalID")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: terminalID, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(terminalID, in);
				else
					strcpy(terminalID, out);
				printf("Node is: terminalID, value=%s\n", terminalID);
                                
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"terminalHD")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: terminalHD, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(terminalHD, in);
				else
					strcpy(terminalHD, out);
				printf("Node is: terminalHD, value=%s\n", terminalHD);
                                
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"userSID")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: userSID, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(userSID, in);
				else
					strcpy(userSID, out);
				printf("Node is: userSID, value=%s\n", userSID);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"userCert")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: userCert, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(userCert, in);
				else
					strcpy(userCert, out);
				printf("Node is: userCert, value=%s\n", userCert);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"updateTime")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: updateTime, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(updateTime, in);
				else
					strcpy(updateTime, out);
				printf("Node is: updateTime, value=%s\n", updateTime);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"terminalCltVer")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: terminalCltVer, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(terminalCltVer, in);
				else
					strcpy(terminalCltVer, out);
				printf("Node is: terminalCltVer, value=%s\n", terminalCltVer);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"terminalType")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: terminalType, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(terminalType, in);
				else
					strcpy(terminalType, out);
				printf("Node is: terminalType, value=%s\n", terminalType);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"updateStatus")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: updateStatus, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(updateStatus, in);
				else
					strcpy(updateStatus, out);
				printf("Node is: updateStatus, value=%s\n", updateStatus);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"terminalTypeName")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: terminalTypeName, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(terminalTypeName, in);
				else
					strcpy(terminalTypeName, out);
				printf("Node is: terminalTypeName, value=%s\n", terminalTypeName);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"terminalOS")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: terminalOS, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(terminalOS, in);
				else
					strcpy(terminalOS, out);
				printf("Node is: terminalOS, value=%s\n", terminalOS);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"vpnGateID")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: vpnGateID, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(vpnGateID, in);
				else
					strcpy(vpnGateID, out);
				printf("Node is: vpnGateID, value=%s\n", vpnGateID);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"res1")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: res1, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(res1, in);
				else
					strcpy(res1, out);
				printf("Node is: res1, value=%s\n", res1);
			}
			if(!xmlStrcmp(curr->name, (const xmlChar *)"res2")) {
				char in[FIELD_LEN];    
				char out[FIELD_LEN];    
				strcpy(in, xmlNodeGetContent(data));    
				printf("Node is: res2, value=%s\n", in);
				if ( conv_charset("GBK", "UTF-8", in, strlen(in), out, FIELD_LEN) )  
					strcpy(res2, in);
				else
					strcpy(res2, out);
				printf("Node is: res2, value=%s\n", res2);
			}
		}    
	}    
	xmlFreeDoc(pdoc);
	snprintf(out, 4096, "INSERT INTO T_term_update_info(terminalID, userCert, userSID, updateTime, terminalCltVer, terminalType, updateStatus, terminalTypeName, terminalOS, terminalHD, vpnGateID, res1, res2) values('%s','%s','%s','%s','%s','%s','%s','%s','%s','%s','%s', '%s', '%s')",
			terminalID,  
			userCert, 
			userSID,  
			updateTime, 
			terminalCltVer, 
			terminalType, 
			updateStatus, 
			terminalTypeName,  
			terminalOS, 
			terminalHD,
			vpnGateID, 
			res1,res2);
        
}

#if 0 
int main(int argc, char* argv[])
{
    char out[4096] = {'\0'};
    //xml_parse(sample_xml, out);
    parse_xml(sample_xml, out);

}
#endif
