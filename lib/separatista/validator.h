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
#ifndef SEPARATISTA_VALIDATOR_H
#define SEPARATISTA_VALIDATOR_H

#include <initializer_list>
#include <array>

#include "separatista.h"
#include "element.h"
#include "exception.h"

namespace Separatista
{
	/// Invalid value exception
	class SEPARATISTA_EXTERN InvalidValueException : public Exception
	{
	public:
		InvalidValueException(const wchar_t *pMessage) : Exception(pMessage) { };

#ifdef SEPARATISTA_DEBUG
		InvalidValueException(const wchar_t *pMessage, const wchar_t *pPath, int nLine) :
			Exception(pMessage, pPath, nLine) { };
#endif
	};

	/// Validator interface
	class SEPARATISTA_EXTERN Validator
	{
	public:
		/**
			The one and only purpose of this class.
			@param pValue The value to validate as string.
			@throws InvalidValueException
		*/
		virtual void validate(const wchar_t *pValue) const = 0;

		static void validateMaxText(const wchar_t *pValue, size_t max);

		static void validateFractionDigits(const wchar_t *pValue, size_t mix, size_t max);

		static void validateTotalDigits(const wchar_t *pValue, size_t min, size_t max);

		static void validateNumeric(const wchar_t *pValue);

		static void validateEnum(const wchar_t *pValue, std::initializer_list<const wchar_t*> pPossibleValues);

	private:
		static void isDigit(const wchar_t c);

		static std::array<wchar_t, 10> m_numericDigits;
	};

	/// Max35Text
	class SEPARATISTA_EXTERN Max35TextValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// ISODateTimeValidator
	class SEPARATISTA_EXTERN ISODateTimeValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// Max15NumericText
	class SEPARATISTA_EXTERN Max15NumericTextValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// DecimalNumber
	class SEPARATISTA_EXTERN DecimalNumberValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// Max140Text
	class SEPARATISTA_EXTERN Max140TextValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// PaymentMethod2CodeValidator
	class SEPARATISTA_EXTERN PaymentMethod2CodeValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// ISODateValidator
	class SEPARATISTA_EXTERN ISODateValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// ChargeBearerType1CodeValidator
	class SEPARATISTA_EXTERN ChargeBearerType1CodeValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// ExternalServiceLevel1CodeValidator
	class SEPARATISTA_EXTERN ExternalServiceLevel1CodeValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// ExternalLocalInstrumentCodeValidator
	class SEPARATISTA_EXTERN ExternalLocalInstrumentCodeValidator : public Max35TextValidator { };

	/// SequenceType3CodeValidator
	class SEPARATISTA_EXTERN SequenceType3CodeValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// ActiveOrHistoricCurrencyAndAmountValidator
	class SEPARATISTA_EXTERN ActiveOrHistoricCurrencyAndAmountValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};
	
	/// ExternalPersonIdentification1Code
	class SEPARATISTA_EXTERN ExternalPersonIdentification1CodeValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// IBANValidator
	class SEPARATISTA_EXTERN IBANValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	/// BICValidator
	class SEPARATISTA_EXTERN BICValidator : public Validator
	{
	public:
		void validate(const wchar_t *pValue) const;
	};

	typedef struct
	{
		Max35TextValidator Max35TextValidator;
		ISODateTimeValidator ISODateTimeValidator;
		Max15NumericTextValidator Max15NumericTextValidator;
		DecimalNumberValidator DecimalNumberValidator;
		Max140TextValidator Max140TextValidator;
		PaymentMethod2CodeValidator PaymentMethod2CodeValidator;
		ISODateValidator ISODateValidator;
		ChargeBearerType1CodeValidator ChargeBearerType1CodeValidator;
		ExternalServiceLevel1CodeValidator ExternalServiceLevel1CodeValidator;
		ExternalLocalInstrumentCodeValidator ExternalLocalInstrumentCodeValidator;
		SequenceType3CodeValidator SequenceType3CodeValidator;
		ActiveOrHistoricCurrencyAndAmountValidator ActiveOrHistoricCurrencyAndAmountValidator;
		ExternalPersonIdentification1CodeValidator ExternalPersonIdentification1CodeValidator;
		IBANValidator IBANValidator;
		BICValidator BICValidator;
	} _Validators;

	static const _Validators Validators;
}

#endif // !defined SEPARATISTA_VALIDATOR_H
