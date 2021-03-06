/*
 *  MessageBus - Inter process communication library
 *  Copyright (C) 2012  Oliver Becker <der.ole.becker@gmail.com>
 * 
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VARIANT_H
#define VARIANT_H

#include <QString>
#include <QVariantMap>
#include <QVariantList>
#include <QByteArray>
#include <QMetaType>

/**

@note setValue() doesn't set the type of the variant, only the = operators and the constructors set the type!

@code
quint64	number	=	100;
Variant	variant(number);
// variant.type() = Variant::UInt64
@endcode

@code
quint16	number	=	100;
Variant	variant;

variant.setType(Variant::ByteArray);
variant.setValue(number);
// variant.type() = Variant::ByteArray
@endcode

@code
quint16	number	=	100;
Variant	variant(number);
// variant.type() = Variant::UInt16

Variant second;
second.setType(Variant::ByteArray);
// second.type() = Variant::ByteArray
second	=	variant;
// second.type() = Variant::UInt16
@endcode
*/
class Variant
{
	public:
		enum Type
		{
			None								=	0x00,
			UInt8								=	0x01,
			Int8								=	0x02,
			UInt16							=	0x03,
			Int16								=	0x04,
			UInt32							=	0x05,
			Int32								=	0x06,
			UInt64							=	0x07,
			Int64								=	0x08,
			ByteArray						=	0x09,
			String							=	0x10,
			Bool								=	0x11,
			SocketDescriptor		=	0x12,
			Map                 = 0x13,
      List                = 0x14
		};
		
	public:
		Variant();
		
		Variant(Variant::Type type);
		
		Variant(qint8 num);
		
		Variant(quint8 num);
		
		Variant(qint16 num);
		
		Variant(quint16 num);
		
		Variant(qint32 num);
		
		Variant(quint32 num);
		
		Variant(qint64 num);
		
		Variant(quint64 num);
		
		Variant(const QByteArray& data);
		
		/**
			@brief Constructs an Variant with the specified data and type.
			
			@warning Use with caution as the data is not checked!
		*/
		Variant(const QByteArray& data, Variant::Type type, quint32 optId = 0);
		
		Variant(const QString& string);
		
		Variant(bool boolean);
    
    Variant(const QVariantMap& map);
    
    Variant(const QVariantList& list);
    
    Variant(const QList<Variant>& list);
		
		Variant(const Variant& other);
		
		Variant(const QVariant& other);
		
		~Variant();
		
		Variant& operator = (const Variant& other);
		
		Variant& operator = (const QVariant& other);
		
		Variant& operator = (const QByteArray& other);
		
		Variant& operator = (const QString& other);
    
    Variant& operator = (const QVariantMap& other);
    
    Variant& operator = (const QVariantList& other);
    
    Variant& operator = (const QList<Variant>& other);
		
		Variant& operator = (bool boolean);
		
		Variant& operator = (quint8 num);
		
		Variant& operator = (qint8 num);
		
		Variant& operator = (quint16 num);
		
		Variant& operator = (qint16 num);
		
		Variant& operator = (quint32 num);
		
		Variant& operator = (qint32 num);
		
		Variant& operator = (quint64 num);
		
		Variant& operator = (qint64 num);

    bool operator == (const Variant& other) const;
		
		bool operator != (const Variant& other) const;
		
		bool isValid() const;
		
		int size() const;
		
		void setOptionalId(quint32 optId);
		
		quint32 optionalId() const;
		
		void setValue(qint8 num);
		
		void setValue(quint8 num);
		
		void setValue(qint16 num);
		
		void setValue(quint16 num);
		
		void setValue(qint32 num);
		
		void setValue(quint32 num);
		
		void setValue(qint64 num);
		
		void setValue(quint64 num);
		
		void setValue(const QByteArray& value);
		
		void setValue(const QString& value);
    
    void setValue(const QVariantMap& value);
    
    void setValue(const QVariantList& value);
    
    void setValue(const QList<Variant>& value);
		
		void setValue(bool boolean);
		
		qint8 toInt8(bool * ok = 0) const;
		
		quint8 toUInt8(bool * ok = 0) const;
		
		qint16 toInt16(bool * ok = 0) const;
		
		quint16 toUInt16(bool * ok = 0) const;
		
		qint32 toInt32(bool * ok = 0) const;
		
		quint32 toUInt32(bool * ok = 0) const;
		
		qint64 toInt64(bool * ok = 0) const;
		
		quint64 toUInt64(bool * ok = 0) const;
		
		int toSocketDescriptor(bool * ok = 0) const;
		
		QByteArray toByteArray(bool * ok = 0) const;
		
		QString toString(bool * ok = 0) const;
    
    QVariantMap toQMap(bool * ok = 0) const;
    
    QVariantList toQList(bool * ok = 0) const;
    
    QList<Variant> toList(bool * ok = 0) const;
    
    QVariant toQVariant(bool * ok = 0) const;
		
		bool toBool(bool * ok = 0) const;

    Type type() const;
		
	public:
		static Variant fromUInt8(quint8 num);
		
		static Variant fromInt8(qint8 num);
		
		static Variant fromUInt16(quint16 num);
		
		static Variant fromInt16(qint16 num);
		
		static Variant fromUInt32(quint32 num);
		
		static Variant fromInt32(qint32 num);
		
		static Variant fromUInt64(quint64 num);
		
		static Variant fromInt64(qint64 num);
		
		static Variant fromSocketDescriptor(int socketDescriptor, bool autoCloseAndDup = false);
		
		static Variant fromByteArray(const QByteArray& data);
    
    static Variant fromByteArray(const char* data, int size);
		
		static Variant fromString(const QString& string);
    
    static Variant fromMap(const QVariantMap& map);
    
    static Variant fromList(const QVariantList& list);
    
    static Variant fromList(const QList<Variant>& list);
		
	private:
		quint64 getUIntNumber(quint8 size, bool * ok) const;
		
		qint64 getIntNumber(quint8 size, bool * ok) const;
    
    void writeIntelligentNumber(QByteArray& target, quint64 num, quint8 shift = 0);
    
    quint64 readIntelligentNumber(const QByteArray& source, int idx, int& readBytes);
		
	private:
		Type							m_type;
		QByteArray				m_data;
		quint32						m_optId;
    bool              m_autoCloseAndDup;
};

Q_DECLARE_METATYPE(Variant)
Q_DECLARE_METATYPE(QList<Variant>)


#endif // HIVARIANT_H
