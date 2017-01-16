// Copyright 2005-2017 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CHANNEL_H_
#define MUMBLE_CHANNEL_H_

#include <QtCore/QHash>
#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtCore/QReadWriteLock>
#include <QtCore/QSet>
#include <QtCore/QString>

class User;
class Group;
class ChanACL;

class ClientUser;

class Channel : public QObject {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(Channel)
	private:
		QSet<Channel *> qsUnseen;
	public:
		int iId;
		int iPosition;
		bool bTemporary;
		Channel *cParent;
		QString qsName;
		QString qsDesc;
		QByteArray qbaDescHash;
		QList<Channel *> qlChannels;
		QList<User *> qlUsers;
		QHash<QString, Group *> qhGroups;
		QList<ChanACL *> qlACL;

		QSet<Channel *> qsPermLinks;
		QHash<Channel *, int> qhLinks;

		bool bInheritACL;

		/// Maximum number of users allowed in the channel. If this
		/// value is zero, the maximum number of users allowed in the
		/// channel is given by the server's "usersperchannel"
		/// setting.
		unsigned int uiMaxUsers;

		Channel(int id, const QString &name, QObject *p = NULL);
		~Channel();

#ifdef MUMBLE
		unsigned int uiPermissions;

		/// List of possible visibility properties for channels
		/// when filtering is active. Only append to this enum
		/// as its values are used in the client DB.
		enum FilteredVisibility {
			/// Channel has normal behavior when filtering, it might get hidden if unoccupied
			FILTERED_VISIBILITY_NORMAL,
			/// Channel is never visible when filtering is active, even when occupied
			FILTERED_VISIBILITY_NEVER,
			/// Channel is always visible when filtering is active, even when empty
			FILTERED_VISIBILITY_ALWAYS,
		};
		
		/// Visibility properties of this channel when filtering is active
		FilteredVisibility filteredVisibility;
		
		static QHash<int, Channel *> c_qhChannels;
		static QReadWriteLock c_qrwlChannels;

		static Channel *get(int);
		static Channel *add(int, const QString &);
		static void remove(Channel *);
		
		/// Returns true if this channel or one of its subchannels has a visibility of FILTERED_VISIBILITY_ALWAYS.
		bool isAlwaysVisibleWhenFiltering() const;
		/// Returns true if this channel or one of its parent channels has a visibility of FILTERED_VISIBILITY_NEVER.
		/// Note: You have to check isAlwaysVisibleWhenFiltering to ensure a channel isn't forced visible by a subchannel.
		bool isNeverVisibleWhenFiltering() const;

		void addClientUser(ClientUser *p);
#endif // MUMBLE
		
		bool isEmpty();
		bool hasAnyUsersInOrBelow();
		bool isUserInOrBelow(const User *user) const;
		
		static bool lessThan(const Channel *, const Channel *);

		size_t getLevel() const;
		size_t getDepth() const;
		QString getPath() const;

		void addChannel(Channel *c);
		void removeChannel(Channel *c);
		void addUser(User *p);
		void removeUser(User *p);

		bool isLinked(Channel *c) const;
		void link(Channel *c);
		void unlink(Channel *c = NULL);

		QSet<Channel *> allLinks();
		QSet<Channel *> allChildren();

		operator QString() const;
};

#endif
