<!--
  - Style sheets for generating the englishbreakfastnetwork website
  - from krazy's xml output.
  -
  - Copyright 2009-2010 by Bertjan Broeksema <b.broeksema@kdemail.net>
  -
  - This program is free software; you can redistribute it and/or modify
  - it under the terms of the GNU General Public License as published by
  - the Free Software Foundation; either version 2 of the License, or
  - (at your option) any later version.
  -
  - This program is distributed in the hope that it will be useful,
  - but WITHOUT ANY WARRANTY; without even the implied warranty of
  - MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
  - GNU General Public License for more details.
  -
  - You should have received a copy of the GNU General Public License along
  - with this program; if not, write to the Free Software Foundation, Inc.,
  - 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
-->
<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" 
  xmlns:xsd="http://www.w3.org/2001/XMLSchema"
  xmlns:ebn="http://www.englishbreakfastnetwork.org/krazy" version="2.0">
  
  <xsl:import href="globalvars.xsl" />

  <!--
    - Returns the number of checkers run for given fileType.
    -
    - @p fileType The fileType for which to count the number of checkers run.
    -             Give 'all' when you want the total number of checkers run.
    -->
  <xsl:function name="ebn:checkerCount" as="xsd:integer">
    <xsl:param name="fileType" as="xsd:string" />
    <xsl:sequence 
      select="if ($fileType eq 'all')
              then count( $global.doc/tool-result/file-types/file-type/check )
              else count( $global.doc/tool-result/file-types/file-type[@value=$fileType]/check )" />
  </xsl:function>
  
  <!--
    - Returns the number of files which have issues.
    -
    - @p fileType The fileType for which to count the number of files having
    -             issues. Give 'all' to get the total number of files which have
    -             issues.
    -->
  <xsl:function name="ebn:numberOfFilesWithIssues" as="xsd:integer">
    <xsl:param name="fileType" as="xsd:string" />
    <xsl:sequence 
      select="if ($fileType eq 'all')
              then count( $global.doc/tool-result/file-types/file-type/check/file )
              else count( $global.doc/tool-result/file-types/file-type[@value=$fileType]/check/file )" />
  </xsl:function>

  <xsl:function name="ebn:issueCount" as="xsd:integer">
    <xsl:param name="fileType" as="xsd:string" />
    <xsl:param name="check" as="xsd:string" />
    
    <xsl:choose>
      <xsl:when test="$fileType ne 'all' and $check eq 'all'" >
        <xsl:sequence 
          select="count( $global.doc/tool-result/file-types/file-type[@value=$fileType]/check/file/issues/line )" />
      </xsl:when>
      <xsl:when test="$fileType eq 'all' and $check ne 'all'" >
        <xsl:sequence
          select="count( $global.doc/tool-result/file-types/file-type/check[@desc=$check]/file/issues/line )" />
      </xsl:when>
      <xsl:when test="$fileType ne 'all' and $check ne 'all'" >
        <xsl:sequence 
          select="count( $global.doc/tool-result/file-types/file-type[@value=$fileType]
                           /check[@desc=$check]/file/issues/line )" />
      </xsl:when>
      <xsl:otherwise>
        <xsl:sequence
          select="count( $global.doc/tool-result/file-types/file-type/check/file/issues/line )" />
      </xsl:otherwise>
    </xsl:choose>
  </xsl:function>

  <!--
    - Returns the total number of files processed by krazy.
    -->
  <xsl:function name="ebn:processedFilesCount" as="xsd:integer">
    <xsl:value-of select="$global.doc/tool-result/global/processed-files/@value" />
  </xsl:function>
  
  <!--
    - Returns the date on which the krazy report was generated.
    -->
  <xsl:function name="ebn:dateOfRun" as="xsd:string">
    <xsl:value-of select="$global.doc/tool-result/global/date/@value" />
  </xsl:function>
</xsl:stylesheet>

<!-- kate:space-indent on; -->
