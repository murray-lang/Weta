#!/bin/bash
java \
 -Djavax.xml.parsers.DocumentBuilderFactory=org.apache.xerces.jaxp.DocumentBuilderFactoryImpl \
 -Djavax.xml.parsers.SAXParserFactory=org.apache.xerces.jaxp.SAXParserFactoryImpl \
 -Dorg.apache.xerces.xni.parser.XMLParserConfiguration=org.apache.xerces.parsers.XIncludeParserConfiguration \
 org.apache.xalan.xslt.Process \
 -in ./docbook/en/TheWetaProject.xml \
 -xsl ./custom.xsl \
 -param collect.xref.targets "only" \
 -param targets.filename "/home/murray/Robotics/Weta/wetaproject.org/wwwroot/doc/docbook/en/target.db"
