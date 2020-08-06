<?xml version='1.0'?>
<xsl:stylesheet
        xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

    <xsl:import href="/home/murray/Apps/docbook-xsl/html/chunk.xsl"/>

    <xsl:param name="html.stylesheet" select="'style.css'"/>
    <xsl:param name="current.docid" select="/*/@xml:id"/>
    <xsl:param name="chapter.autolabel">0</xsl:param>
    <xsl:param name="use.id.as.filename">1</xsl:param>
    <xsl:param name="para.propagates.style">1</xsl:param>
    <xsl:param name="emphasis.propagates.style">1</xsl:param>
    <xsl:param name="generate.toc">
        appendix  nop
        article   nop
        book      toc,title,figure,table,example,equation
        chapter   toc
        part      toc
        preface   nop
        qandadiv  nop
        qandaset  nop
        reference toc,title
        section   nop
        set       toc
    </xsl:param>
    <xsl:param name="toc.section.depth">1</xsl:param>

    <xsl:param name="formal.title.placement">
        figure before
        example before
        equation before
        table after
        procedure before
    </xsl:param>

</xsl:stylesheet>
