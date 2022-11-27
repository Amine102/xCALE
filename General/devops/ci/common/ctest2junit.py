#!/usr/bin/python3

from lxml import etree
import sys

if len(sys.argv) < 3:
    print("""The first arg should be the path to the build folder, the second arg should be the junit xsl template file (you can find it in devops/ci/common).""")
    exit(1)

# We need the XML File stored in a directory referenced by the first line of the TAG file
the_TAG_file = open(sys.argv[1] + '/Testing/TAG', 'r')
the_XML_folder = the_TAG_file.readline().strip()

the_XML_fs = open(sys.argv[1] + '/Testing/' + the_XML_folder + '/Test.xml', 'r')
the_XSL_fd = open(sys.argv[2], 'r')

the_XML_data = the_XML_fs.read()
the_XSL_data = the_XSL_fd.read()

xslt_transformation = etree.XSLT(etree.XML(the_XSL_data))

the_XML_data_as_junit_format = xslt_transformation(etree.fromstring(the_XML_data.encode('utf-8'), 
                                                                    parser=etree.XMLParser(ns_clean=True, 
                                                                                           recover=True, 
                                                                                           encoding='utf-8')))

print(the_XML_data_as_junit_format)

