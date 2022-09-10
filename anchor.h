#ifndef ANCHOR_H
#define ANCHOR_H

#include <QString>

class Anchor
{
public:
    Anchor();
    Anchor(uint32_t anchor_id,
           QString ip_addr,
           QString description,
           QString server_ip,
           uint32_t server_port);

    uint32_t getAnchorId() const;
    void setAnchorId(uint32_t anchor_id);
    QString getIpAddr() const;
    void setIpAddr(QString ip_addr);
    QString getDescription() const;
    void setDescription(QString description);
    QString getServerIp() const;
    void setServerIp(QString server_ip);
    uint32_t getServerPort() const;
    void setServerPort(uint32_t server_port);
private:
    uint32_t m_anchor_id = 0;
    QString m_ip_addr = "127.0.0.1";
    QString m_description = "";
    QString m_server_ip = "127.0.0.1";
    uint32_t m_server_port = 5555;
};

#endif // ANCHOR_H
