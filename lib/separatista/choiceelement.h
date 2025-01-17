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

#ifndef SEPARATISTA_CHOICEELEMENT_H
#define SEPARATISTA_CHOICEELEMENT_H

#include <array>
#include <ctime>
#include <string>
#include <initializer_list>
#include <algorithm>

#include "separatista/separatista.h"
#include "separatista/xerces_types.h"
#include "separatista/element.h"
#include "separatista/exception.h"
#include "separatista/debug/debug.h"
#include "separatista/documentiterator.h" 

namespace Separatista
{
	class InvalidChoiceException : public Exception
	{
	public:
		InvalidChoiceException(const wchar_t *pMessage) : Exception(pMessage) { };

#ifdef SEPARATISTA_DEBUG
		InvalidChoiceException(const wchar_t *pMessage, const wchar_t *pPath, int line) : Exception(pMessage, pPath, line) { };
#endif
	};

	/**
		Choice elements require to choose a child element to be written on save. If unset, the element won't be outputted.
		Choice element child element should always be optional (!!)
	*/

	template <size_t T>
	class ChoiceElement : public Element
	{
	public:
		ChoiceElement(const wchar_t *pTagName, std::initializer_list<Element*> pChoices, const ElementOptions options) :
			Element(pTagName, options)
			{
				DEBUG_METHOD

				m_pChosenElement = NULL;
				std::copy(pChoices.begin(), pChoices.end(), m_pChoices.begin());
			};

		/**
			@return The element created or NULL if no element was choosen.
		*/

		DOMElement* toDOMDocument(DOMDocument *pDocument, DOMElement *pParent, const ErrorOptions errorOptions = ThrowExceptions)
		{
			DEBUG_METHOD

			if (!m_pChosenElement)
				return NULL;

			DOMElement *pElement = createElement(pDocument, pParent);

			if (pElement)
			{
				m_pChosenElement->toDOMDocument(pDocument, pElement, errorOptions);
			}

			return pElement;
		};

		/**
			Iterates over the possible choices and calls fromDOMDocument on the chosen element.
		*/
		void fromDOMDocument(DOMDocumentIterator &documentIterator, const Element::ErrorOptions errorOptions = ThrowExceptions)
		{
			DEBUG_METHOD

			for (auto it = m_pChoices.begin(); it != m_pChoices.end(); it++)
			{
				// Find the chosen element by setting all values and catching the exception
				try
				{
					if (documentIterator.fromDOMDocument(**it, errorOptions))
					{
						// Element was found
						m_pChosenElement = *it;
						return;
					}
				}
				catch (const MissingElementException &)
				{
					// Element is missing
					continue;
				}
			}

			// No choice was found
			if (errorOptions == Element::ThrowExceptions)
				throw(InvalidChoiceException(SEPARATISTA_EXCEPTION("No chosen child element found!")), this);
		}

		/**
			Choose the child element to be the chosen one.
			@param pChildElement Make sure this actually a child element of the choice element. Can be NULL,
			this means this element won't be outputted on toDOMDocument.
			@throws InvalidChoiceException if the chosen element isn't a child of this element.
		*/
		void choose(Element *pChildElement)
		{
			DEBUG_METHOD

			for (auto it = m_pChoices.begin(); it != m_pChoices.end(); it++)
			{
				if (*it == pChildElement)
				{
					m_pChosenElement = pChildElement;
					return;
				}
			}

			throw InvalidChoiceException(SEPARATISTA_EXCEPTION("Chosen element not a child element"));
		};

		/**
			Returns the chosen element.
			@return The chosen element or NULL if no choice was registered.
		*/
		Element* getChoice()
		{
			DEBUG_METHOD

			return m_pChosenElement;
		};

	private:
		/// The chosen element 
		Element *m_pChosenElement;
		/// Array containing all choices
		std::array<Element*, T> m_pChoices;

	};
}
#endif // SEPARATISTA_CHOICEELEMENT_H
