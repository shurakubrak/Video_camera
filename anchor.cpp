#include "anchor.h"

Anchor::Anchor()
{

}

Anchor::Anchor(uint32_t anchor_id,
       QString ip_addr,
       QString description,
       QString server_ip,
       uint32_t server_port)
{
    m_anchor_id = anchor_id;
    m_ip_addr = ip_addr;
    m_description = description;
    m_server_ip = server_ip;
    m_server_port = server_port;
}

uint32_t Anchor::getAnchorId() const
{
    return m_anchor_id;
}
void Anchor::setAnchorId(uint32_t anchor_id)
{
    m_anchor_id = anchor_id;
}

QString Anchor::getIpAddr() const
{
    return m_ip_addr;
}
void Anchor::setIpAddr(QString ip_addr)
{
    m_ip_addr = ip_addr;
}

QString Anchor::getDescription() const
{
    return m_description;
}
void Anchor::setDescription(QString description)
{
    m_description = description;
}

QString Anchor::getServerIp() const
{
    return m_server_ip;
}
void Anchor::setServerIp(QString server_ip)
{
    m_server_ip = server_ip;
}

uint32_t Anchor::getServerPort() const
{
    return m_server_port;
}
void Anchor::setServerPort(uint32_t server_port)
{
    m_server_port = server_port;
}
