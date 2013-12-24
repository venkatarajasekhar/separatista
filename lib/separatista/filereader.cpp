/***************************************************************************
*   Copyright (C) 2013 by Okkel Klaver   *
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

#include <xercesc/parsers/XercesDOMParser.hpp>
#include <xercesc/dom/DOM.hpp>
#include <xercesc/sax/HandlerBase.hpp>
#include <xercesc/util/XMLString.hpp>

#include <windows.h>
#include "separatista.h"
#include "iban/iban.h"

using namespace xercesc;
using namespace Separatista;

SeparatistaFileReader::SeparatistaFileReader()
{
	m_path = NULL;
	m_pErrorMessage = NULL;
	m_pDocument = NULL;
	m_status = E_FILE;
}

SeparatistaFileReader::~SeparatistaFileReader()
{
	if (m_pErrorMessage)
		XMLString::release(&m_pErrorMessage);
	if (m_path)
		XMLString::release(&m_path);
}

const wchar_t* SeparatistaFileReader::getErrorMessage() const
{
	return m_pErrorMessage;
}

void SeparatistaFileReader::setErrorMessage(const wchar_t *pErrorMessage)
{
	if (m_pErrorMessage)
		XMLString::release(&m_pErrorMessage);

	m_pErrorMessage = XMLString::replicate(pErrorMessage);
}

const SeparatistaFileReader::DocumentStatus SeparatistaFileReader::getStatus() const
{
	return m_status;
}

void SeparatistaFileReader::setStatus(const SeparatistaFileReader::DocumentStatus status)
{
	m_status = status;
}

SeparatistaDocument* SeparatistaFileReader::getDocument() const
{
	return m_pDocument;
}

void SeparatistaFileReader::setDocument(SeparatistaDocument *pDocument)
{
	m_pDocument = pDocument;
}

const wchar_t* SeparatistaFileReader::getPath() const
{
	return m_path;
}

const SeparatistaFileReader::DocumentStatus SeparatistaFileReader::readDocument(const wchar_t *path)
{
	XercesDOMParser *parser;
	ErrorHandler *handler;
	SeparatistaDocument *pDocument;
	xercesc::DOMDocument *pDOMDocument;

	m_status = OK;

	if (m_path)
		XMLString::release(&m_path);
	m_path = XMLString::replicate(path);

	parser = new XercesDOMParser();
	if (!parser)
	{
		m_status = E_MEMORY;
		return m_status;
	}

	parser->setValidationScheme(XercesDOMParser::Val_Always);
	parser->setDoNamespaces(true);

	handler = (ErrorHandler*)new HandlerBase();
	if (!handler)
	{
		delete parser;
		m_status = E_MEMORY;
		return m_status;
	}
	parser->setErrorHandler(handler);

	try
	{
		parser->parse(path);

		// Try to convert the document into the right separatista class
		pDOMDocument = parser->getDocument();
		if (pDOMDocument != NULL && (pDocument = SeparatistaDocument::getByDOMDocument(parser->getDocument())) != NULL)
		{
			pDocument->setPath(path);
			m_pDocument = pDocument;
		}
		else
			m_status = E_FORMAT;
	}
	catch (const XMLException &e)
	{
		m_pErrorMessage = XMLString::replicate(e.getMessage());
		m_status = E_FORMAT;
	}
	catch (const DOMException &e)
	{
		m_pErrorMessage = XMLString::replicate(e.getMessage());
		m_status = E_FORMAT;
	}
	catch (...)
	{
		m_status = E_FORMAT;
	}
	
	delete parser;
	delete handler;

	return m_status;
}
