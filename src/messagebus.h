// Copyright 2012 Oliver Becker <der.ole.becker@gmail.com>
// 
// This file is part of the MessageBus project,
// 
// MessageBus is free software: you can redistribute it and/or modify it
// under the terms of the GNU General Public License as published by the Free Software Foundation,
// either version 3 of the License, or (at your option) any later version.
// MessageBus is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
// without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
// See the GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License along with MessageBus.
// If not, see http://www.gnu.org/licenses/.

#ifndef MESSAGEBUS_H
#define MESSAGEBUS_H

#include <QtCore>

#include "variant.h"

class MessageBusPrivate;

class LocalSocket;

class MessageBus : public QObject
{
	Q_OBJECT
	
	friend class MessageBusPrivate;
	friend class MessageBusInterfacePrivate;

	public:
		MessageBus(const QString& service, const QString& object, QObject * parent);

		virtual ~MessageBus();

		///@warning callRet() must not be called within a function that is called by callRet() itself!
		Variant callRet(const QString& slot, const Variant& var1 = Variant(), const Variant& var2 = Variant(), const Variant& var3 = Variant(), const Variant& var4 = Variant());
		
		Variant callRet(const QString& slot, const QList<Variant>& args);

		void call(const QString& slot, const Variant& var1 = Variant(), const Variant& var2 = Variant(), const Variant& var3 = Variant(), const Variant& var4 = Variant(), const Variant& var5 = Variant());
		
		void call(const QString& slot, const QList<Variant>& args);
		
		virtual bool open();
		
		virtual void close();
		
		bool isOpen() const;
		
	signals:
		void disconnected();
		
	private slots:
		void onNewPackage();
		
		void onDisconnected();
		
	private:
		MessageBus(QObject * target, LocalSocket * socket, QObject * parent);

	private:
		MessageBusPrivate			*	d;
};

#endif // MESSAGEBUS_H