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
<?xml version="1.0" encoding="utf-8"?>
<xsl:stylesheet version="2.0" 
  xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
  xmlns:xsd="http://www.w3.org/2001/XMLSchema"
  exclude-result-prefixes="xsl xsd">
  
  <!-- Mode: krazy2ebn -->
  <xsl:import href="krazy2ebn.xsl" />
  
  <xsl:template match="/krazy">
    <error>unsupported mode!</error>
  </xsl:template>
</xsl:stylesheet>