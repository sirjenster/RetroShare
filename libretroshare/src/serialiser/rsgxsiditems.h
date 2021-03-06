/*
 * libretroshare/src/serialiser: rsgxsiditems.h
 *
 * RetroShare C++ Interface.
 *
 * Copyright 2012-2012 by Robert Fernie
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License Version 2 as published by the Free Software Foundation.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA.
 *
 * Please report all bugs and problems to "retroshare@lunamutt.com".
 *
 */

#ifndef RS_GXS_IDENTITY_ITEMS_H
#define RS_GXS_IDENTITY_ITEMS_H

#include <map>

#include "serialiser/rsserviceids.h"
#include "serialiser/rsserial.h"

#include "rsgxsitems.h"
#include "retroshare/rsidentity.h"

//const uint8_t RS_PKT_SUBTYPE_GXSID_GROUP_ITEM_deprecated   = 0x02;

const uint8_t RS_PKT_SUBTYPE_GXSID_GROUP_ITEM      = 0x02;
const uint8_t RS_PKT_SUBTYPE_GXSID_OPINION_ITEM    = 0x03;
const uint8_t RS_PKT_SUBTYPE_GXSID_COMMENT_ITEM    = 0x04;
const uint8_t RS_PKT_SUBTYPE_GXSID_LOCAL_INFO_ITEM = 0x05;

class RsGxsIdItem: public RsGxsGrpItem
{
    public:
        RsGxsIdItem(uint8_t item_subtype) : RsGxsGrpItem(RS_SERVICE_GXS_TYPE_GXSID,item_subtype) {}

        virtual bool serialise(void *data,uint32_t& size) = 0 ;
        virtual uint32_t serial_size() = 0 ;

        virtual void clear() = 0 ;
        virtual std::ostream& print(std::ostream &out, uint16_t indent = 0) = 0;

    bool serialise_header(void *data,uint32_t& pktsize,uint32_t& tlvsize, uint32_t& offset) ;
};

class RsGxsIdGroupItem : public RsGxsIdItem
{
public:

    RsGxsIdGroupItem():  RsGxsIdItem(RS_PKT_SUBTYPE_GXSID_GROUP_ITEM) {}
    virtual ~RsGxsIdGroupItem() {}

    virtual void clear();
    virtual std::ostream &print(std::ostream &out, uint16_t indent = 0);

    virtual bool serialise(void *data,uint32_t& size) ;
    virtual uint32_t serial_size() ;

    bool fromGxsIdGroup(RsGxsIdGroup &group, bool moveImage);
    bool toGxsIdGroup(RsGxsIdGroup &group, bool moveImage);

    Sha1CheckSum mPgpIdHash;
    // Need a signature as proof - otherwise anyone could add others Hashes.
    // This is a string, as the length is variable.
    std::string mPgpIdSign;

    // Recognition Strings. MAX# defined above.
    std::list<std::string> mRecognTags;

    // Avatar
    RsTlvImage mImage ;
};
class RsGxsIdLocalInfoItem : public RsGxsIdItem
{

public:

    RsGxsIdLocalInfoItem():  RsGxsIdItem(RS_PKT_SUBTYPE_GXSID_LOCAL_INFO_ITEM) {}
    virtual ~RsGxsIdLocalInfoItem() {}

    virtual void clear();
    virtual std::ostream &print(std::ostream &out, uint16_t indent = 0);

    virtual bool serialise(void *data,uint32_t& size) ;
    virtual uint32_t serial_size() ;

    std::map<RsGxsId,time_t> mTimeStamps ;
};

#if 0
class RsGxsIdOpinionItem : public RsGxsMsgItem
{
public:

    RsGxsIdOpinionItem(): RsGxsMsgItem(RS_SERVICE_GXS_TYPE_GXSID,
			RS_PKT_SUBTYPE_GXSID_OPINION_ITEM) {return; }
        virtual ~RsGxsIdOpinionItem() { return;}
        void clear();
    virtual bool serialise(void *data,uint32_t& size) = 0 ;
    virtual uint32_t serial_size() = 0 ;

    std::ostream &print(std::ostream &out, uint16_t indent = 0);
	RsGxsIdOpinion opinion;
};

class RsGxsIdCommentItem : public RsGxsMsgItem
{
public:

    RsGxsIdCommentItem(): RsGxsMsgItem(RS_SERVICE_GXS_TYPE_GXSID,
                                          RS_PKT_SUBTYPE_GXSID_COMMENT_ITEM) { return; }
    virtual ~RsGxsIdCommentItem() { return; }
    void clear();
    virtual bool serialise(void *data,uint32_t& size) = 0 ;
    virtual uint32_t serial_size() = 0 ;

    std::ostream &print(std::ostream &out, uint16_t indent = 0);
    RsGxsIdComment comment;

};
#endif

class RsGxsIdSerialiser : public RsSerialType
{
public:
    RsGxsIdSerialiser() :RsSerialType(RS_PKT_VERSION_SERVICE, RS_SERVICE_GXS_TYPE_GXSID) {}
    virtual     ~RsGxsIdSerialiser() {}

    virtual uint32_t 	size (RsItem *item)
    {
        RsGxsIdItem *idItem = dynamic_cast<RsGxsIdItem *>(item);
        if (!idItem)
        {
            return 0;
        }
        return idItem->serial_size() ;
    }
    virtual bool serialise(RsItem *item, void *data, uint32_t *size)
    {
        RsGxsIdItem *idItem = dynamic_cast<RsGxsIdItem *>(item);
        if (!idItem)
        {
            return false;
        }
        return idItem->serialise(data,*size) ;
    }
    virtual RsItem *deserialise (void *data, uint32_t *size) ;

private:
#if 0
    static RsGxsIdOpinionItem *deserialise_GxsIdOpinionItem(void *data, uint32_t *size);
    static RsGxsIdCommentItem *deserialise_GxsIdCommentItem(void *data, uint32_t *size);
#endif
    static RsGxsIdGroupItem   *deserialise_GxsIdGroupItem(void *data, uint32_t *size);
    static RsGxsIdLocalInfoItem   *deserialise_GxsIdLocalInfoItem(void *data, uint32_t *size);
};

#endif /* RS_GXS_IDENTITY_ITEMS_H */
