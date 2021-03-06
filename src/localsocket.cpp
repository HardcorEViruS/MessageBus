/*
 *  MessageBus - Inter process communication library
 *  Copyright (C) 2013  Oliver Becker <der.ole.becker@gmail.com>
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

#include "localsocket.h"

#include <QThread>
#include <QTimer>
#include "eventloop.h"

#if defined(Q_OS_UNIX) || defined(Q_OS_LINX)
	#include "implementations/localsocketprivate_unix.h"
#else
	#error No implementation of LocalSocket for this operating system!
#endif

LocalSocket::LocalSocket(QObject* parent)
	:	QObject(parent),
#if defined(Q_OS_UNIX) || defined(Q_OS_LINX)
	d_ptr(new LocalSocketPrivate_Unix(this))
#else
	#error No implementation of LocalSocket for this operating system!
#endif
{
  connect(d_ptr, SIGNAL(readyRead()), SIGNAL(readyRead()));
  connect(d_ptr, SIGNAL(error(QString)), SIGNAL(error(QString)));
  connect(d_ptr, SIGNAL(disconnected()), SIGNAL(disconnected()));
}

LocalSocket::~LocalSocket()
{
	disconnectFromServer();
	
	delete d_ptr;
}


bool LocalSocket::connectToServer(const QString& filename)
{
	if(isOpen() || filename.isEmpty())
		return false;
	
	return d_ptr->connectToServer(filename);
}


bool LocalSocket::setSocketDescriptor(quintptr socketDescriptor)
{
	if(isOpen() || socketDescriptor == 0)
		return false;
	
	return d_ptr->setSocketDescriptor(socketDescriptor);
}


quintptr LocalSocket::socketDescriptor() const
{
	QReadLocker		controlLock(&d_ptr->m_controlLock);
	
	return d_ptr->m_socketDescriptor;
}


bool LocalSocket::isOpen() const
{
	QReadLocker		controlLock(&d_ptr->m_controlLock);
	
	return d_ptr->m_isOpen && d_ptr->m_errorString.isEmpty();
}


Variant LocalSocket::read(bool* ok)
{
	QWriteLocker		readLock(&d_ptr->m_readBufferLock);
	
	if(d_ptr->m_readBuffer.isEmpty())
	{
		if(ok)
			*ok	=	false;
		
		return Variant();
	}
	else
	{
		if(ok)
			*ok	=	true;
		
		return d_ptr->m_readBuffer.takeFirst();
	}
}


int LocalSocket::availableData() const
{
	QReadLocker		readLock(&d_ptr->m_readBufferLock);
	
	return d_ptr->m_readBuffer.count();
}


int LocalSocket::dataToWrite() const
{
	QWriteLocker		writeLock(&d_ptr->m_writeBufferLock);
	
	return d_ptr->m_writeBuffer.count() + (d_ptr->m_currentWriteData.isEmpty() ? 0 : 1);
}


bool LocalSocket::waitForReadyRead(int timeout)
{
	if(!isOpen())
		return false;
	
	{
		QReadLocker		readLock(&d_ptr->m_readBufferLock);
		if(!d_ptr->m_readBuffer.isEmpty())
			return true;
	}
	
	QElapsedTimer	timer;
	timer.start();
	
	if(timeout < 0)
		timeout	=	0;
  
//   // Waiting through event loop
//   QTimer     timeoutTimer;
//   EventLoop  eventLoop;
//   
//   connect(&timeoutTimer, SIGNAL(timeout()), &eventLoop, SLOT(abort()));
//   connect(d_ptr, SIGNAL(readyRead()), &eventLoop, SLOT(quit()));
//   connect(d_ptr, SIGNAL(disconnected()), &eventLoop, SLOT(abort()));
//   
//   timeoutTimer.setInterval(timeout);
//   timeoutTimer.start();
//   
//   return (eventLoop.exec() == 0);
	
	return d_ptr->waitForReadyRead(timer, timeout);
}


bool LocalSocket::waitForDataWritten(int timeout)
{
	if(!isOpen())
		return false;
	
	{
		QWriteLocker		writeLock(&d_ptr->m_writeBufferLock);
		if(d_ptr->m_writeBuffer.isEmpty() && d_ptr->m_currentWriteData.isEmpty())
			return true;
	}
	
	QElapsedTimer	timer;
	timer.start();
	
	if(timeout < 0)
		timeout	=	0;
  
//   // Waiting through event loop
//   QTimer     timeoutTimer;
//   EventLoop  eventLoop;
//   
//   connect(&timeoutTimer, SIGNAL(timeout()), &eventLoop, SLOT(abort()));
//   connect(d_ptr, SIGNAL(bytesWritten()), &eventLoop, SLOT(quit()));
//   connect(d_ptr, SIGNAL(disconnected()), &eventLoop, SLOT(abort()));
//   
//   timeoutTimer.setInterval(timeout);
//   timeoutTimer.start();
//   
//   return (eventLoop.exec() == 0);
	
	return d_ptr->waitForDataWritten(timer, timeout);
}


QString LocalSocket::lastErrorString() const
{
	QReadLocker		controlLock(&d_ptr->m_controlLock);
	
	return d_ptr->m_errorString;
}


void LocalSocket::disconnectFromServer()
{
	if(!isOpen())
		return;
	
	d_ptr->disconnectFromServer();
}


bool LocalSocket::write(const Variant& data)
{
	if(!isOpen())
		return false;
	
	{
		QWriteLocker		writeLock(&d_ptr->m_writeBufferLock);
		d_ptr->m_writeBuffer.append(data);
	}
	
	d_ptr->notifyWrite();
	
	return true;
}


bool LocalSocket::flush()
{
	if(!isOpen())
		return false;
	
	d_ptr->flush();
	
	return isOpen();
}
