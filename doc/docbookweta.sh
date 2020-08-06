#!/bin/bash
java \
 -Djavax.xml.parsers.DocumentBuilderFactory=org.apache.xerces.jaxp.DocumentBuilderFactoryImpl \
 -Djavax.xml.parsers.SAXParserFactory=org.apache.xerces.jaxp.SAXParserFactoryImpl \
 -Dorg.apache.xerces.xni.parser.XMLParserConfiguration=org.apache.xerces.parsers.XIncludeParserConfiguration \
 org.apache.xalan.xslt.Process \
 -in ./docbook/en/TheWetaProject.xml \
 -out ./html/en/TheWetaProject.html \
 -xsl ./custom.xsl \
 -param use.extensions 1 \
 -param target.database.document "/home/murray/Robotics/Weta/wetaproject.org/wwwroot/doc/docbook/en/olinkdb.xml"   
