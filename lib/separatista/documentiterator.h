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

#ifndef SEPARATISTA_DOCUMENTITERATOR_H
#define SEPARATISTA_DOCUMENTITERATOR_H

#include <ctime>
#include <string>

#include "separatista.h"
#include "xerces_types.h"

namespace Separatista
{
	/**
	Class for enumerating a DOMDocument.
	*/
	class SEPARATISTA_EXTERN DOMDocumentIterator
	{
	public:
		DOMDocumentIterator(DOMDocument *pDocument);

		~DOMDocumentIterator();

		DOMElement* getCurrentElement() const;

		DOMElement* nextElement();

		/**
		Get the current position. Can be used to check for dead loops.
		*/
		unsigned int getPosition() const;

	private:
		DOMNodeIterator *m_pNodeIterator;
		DOMNode *m_pCurrentNode;
		unsigned int m_nPos;
	};

}

#endif // !defined SEPARATISTA_DOCUMENTITERATOR_H