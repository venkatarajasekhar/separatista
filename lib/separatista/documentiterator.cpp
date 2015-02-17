/***************************************************************************
*   Copyright (C) 2014 by Okkel Klaver   *
*   info@vanhetland.nl   *
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
*   This program is distributed in the hope that it will be useful,       *
*   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
*   GNU General Public License for more details.                          *
*                                                                         *
*   You should have received a copy of the GNU General Public License     *
*   along with this program; if not, write to the                         *
*   Free Software Foundation, Inc.,                                       *
*   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
***************************************************************************/

#include <string>
#include <sstream>
#include <iomanip>

#include <xercesc/dom/DOMException.hpp>
#include <xercesc/dom/DOMDocument.hpp>
#include <xercesc/dom/DOMElement.hpp>
#include <xercesc/dom/DOMNodeIterator.hpp>
#include <xercesc/dom/DOMNodeList.hpp>
#include <xercesc/util/XMLDateTime.hpp>
#include <xercesc/framework/psvi/XSValue.hpp>

#include "separatista.h"
#include "documentiterator.h"
#include "debug/debug.h"

using namespace Separatista;

DOMDocumentIterator::DOMDocumentIterator(Separatista::DOMDocument *pDocument)
{
	DEBUG_METHOD
	m_pNodeIterator = pDocument->createNodeIterator(pDocument->getDocumentElement(), xercesc::DOMNodeFilter::SHOW_ELEMENT, NULL, true);
	
	if (m_pNodeIterator)
		m_pCurrentNode = m_pNodeIterator->nextNode();
	else
		m_pCurrentNode = NULL;

	m_nPos = 0;
}

DOMDocumentIterator::~DOMDocumentIterator()
{
	DEBUG_METHOD
	if (m_pNodeIterator)
		m_pNodeIterator->release();
}

xercesc::DOMElement* DOMDocumentIterator::getCurrentElement() const
{
	DEBUG_METHOD
	return (xercesc::DOMElement*)m_pCurrentNode;
}

xercesc::DOMElement* DOMDocumentIterator::nextElement()
{
	DEBUG_METHOD
	if (m_pNodeIterator)
	{
		m_pCurrentNode = m_pNodeIterator->nextNode();
		m_nPos++;
	}

	return getCurrentElement();
}

unsigned int DOMDocumentIterator::getPosition() const
{
	DEBUG_METHOD
	return m_nPos;
}