/* ==================================== JUCER_BINARY_RESOURCE ====================================

   This is an auto-generated file: Any edits you make may be overwritten!

*/

namespace BinaryData
{

//================== read.xml ==================
static const unsigned char temp_binary_data_0[] =
"<CATALOG>\n"
"    <CD TITLE=\"Empire Burlesque\"\n"
"        ARTIST=\"Bob Dylan\"\n"
"        COUNTRY=\"USA\"\n"
"        COMPANY=\"Columbia\"\n"
"        PRICE=\"10.90\"\n"
"        YEAR=\"1985\" />\n"
"    <CD TITLE=\"Hide your heart\"\n"
"        ARTIST=\"Bonnie Tyler\"\n"
"        COUNTRY=\"UK\"\n"
"        COMPANY=\"CBS Records\"\n"
"        PRICE=\"9.90\"\n"
"        YEAR=\"1988\" />\n"
"</CATALOG>\n";

const char* read_xml = (const char*) temp_binary_data_0;


const char* getNamedResource (const char* resourceNameUTF8, int& numBytes)
{
    unsigned int hash = 0;

    if (resourceNameUTF8 != nullptr)
        while (*resourceNameUTF8 != 0)
            hash = 31 * hash + (unsigned int) *resourceNameUTF8++;

    switch (hash)
    {
        case 0xcc4f9dee:  numBytes = 332; return read_xml;
        default: break;
    }

    numBytes = 0;
    return nullptr;
}

const char* namedResourceList[] =
{
    "read_xml"
};

const char* originalFilenames[] =
{
    "read.xml"
};

const char* getNamedResourceOriginalFilename (const char* resourceNameUTF8)
{
    for (unsigned int i = 0; i < (sizeof (namedResourceList) / sizeof (namedResourceList[0])); ++i)
    {
        if (namedResourceList[i] == resourceNameUTF8)
            return originalFilenames[i];
    }

    return nullptr;
}

}
