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

#ifndef SEPARATISTA_IBAN_H
#define SEPARATISTA_IBAN_H

#include <string>

#ifdef SEPARATISTA_IBAN_BUILDINGDLL
#	define IBAN_EXTERN __declspec(dllexport)
#elif defined SEPARATISTA_IBAN_USINGDLL
#		define IBAN_EXTERN __declspec(dllimport)
#else
#	define IBAN_EXTERN
#endif

namespace Separatista 
{

typedef char IBANCC[2];

class IBAN
{
public:
	/**
		Constructor
	*/
	IBAN_EXTERN IBAN();

	IBAN_EXTERN IBAN& operator = (const char *iban);

	IBAN_EXTERN const char* getIBAN();

	IBAN_EXTERN operator const char* ();

	IBAN_EXTERN const IBANCC &getCountryCode() const;

	IBAN_EXTERN const IBANCC &getControlSum() const;

	IBAN_EXTERN bool Check() const;

	IBAN_EXTERN void Clear();

protected:
	void set(const char *iban);
	void format();

private:
	IBANCC m_countryCode;
	IBANCC m_controlSum;
	char m_accountNumber[30];
	int m_accountNumberLength;

	std::string m_formatted;
};

}

#endif // !defined SAPARATISTA_IBAN_H