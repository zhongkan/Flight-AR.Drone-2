#include "navData.h"

navData::navData(ARDrone *ardrone, const QString ipAddr, QObject *parent) :
    QThread(parent)
{
    this->_ardrone = ardrone;
    this->_ipAddrStr = ipAddr;
    this->_stop = false;
    this->_setup = true;
}

navData::~navData()
{
    delete this->_ipAddr;
    delete this->_navSocket;
}

void navData::go()
{
	this->_stop = false;
	this->start();
}

void navData::stop()
{
    this->_stop = true;
}

void navData::run()
{
    /* Initialize the ip address */
    this->_ipAddr = new QHostAddress(this->_ipAddrStr);
    this->_navSocket = new QUdpSocket();
    quint32 csk;

    /* Initialize the navdata stream */
    RESULT res = this->initNavData();
    if (M_FAILED(res))
    {
        this->print("Navdata stream initiation FAILED!");
		emit this->initNavStreamRes(NAV_DATA_INIT_FAILED);
        return;
    }
    else
    {
        this->print("Navdata stream initiation OK!");
		emit this->initNavStreamRes(NAV_DATA_INIT_OK);
		emit this->startNavUpdate();
    }

    /* Main loop : receive navdata */
    while (!this->_stop)
    {
		if (this->_navSocket->waitForReadyRead(2000))
		{
            this->_datagram.resize(this->_navSocket->pendingDatagramSize());
            this->_navSocket->readDatagram(this->_datagram.data(), this->_datagram.size());

            if (!(this->getState(this->_datagram) & ARDRONE_NAVDATA_BOOTSTRAP) && this->isNavPkt(this->_datagram))
            {
                this->_navdata = (navdata_t*)this->_datagram.data();
                res = this->ardrone_navdata_unpack_all(&this->_navdata_unpacked, this->_navdata, &csk);
                if (M_FAILED(res)) continue;
            }
            else
            {
                this->print("Read navdata TIMEOUT!");
                continue;
            }
        }
    }

	delete _ipAddr;
	delete _navSocket;
}

RESULT navData::initNavData()
{
    bool ans;
    char data[5] = {0x01, 0x01, 0x01, 0x01, 0x00};
    QByteArray datagram = QByteArray(data);
    int trytimes = 1;

    ans = this->_navSocket->bind(NAV_DATA_PORT, QUdpSocket::DontShareAddress);
    if (!ans)
     {
         this->print("Binding Socket to NAV_DATA_PORT FAILED!");
         return A_FAILED;
     }
     else
     {
         this->print("Binding Socket to NAV_DATA_PORT OK!");
     }

    /* write one packet to adrone on port NAV_DATA_PORT */
    this->_navSocket->writeDatagram(datagram, *(this->_ipAddr), NAV_DATA_PORT);

    /* Receive ARDrone status, wait for 1 seconds to timeout */
    while(true)
    {
        if (this->_navSocket->waitForReadyRead(1000))
        {
            QByteArray rec_datagram;
            rec_datagram.resize(this->_navSocket->pendingDatagramSize());
            this->_navSocket->readDatagram(rec_datagram.data(), rec_datagram.size());

            if (!this->isNavPkt(rec_datagram)) // check it is a navdata datagram
                continue;

            uint state = this->getState(rec_datagram);

            if (state & ARDRONE_COMMAND_MASK)
            {
                 string cmd = "AT*CTRL=" + itos(this->_ardrone->getSeqNum()) + ",5,0";
                 this->_ardrone->sendAT_CMD(cmd);
                 msleep(INTERVAL);
            }

			this->_ardrone->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", _ardrone->getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
            this->_ardrone->configure("pic:ultrasound_freq", "8");
            msleep(INTERVAL);

			this->_ardrone->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", _ardrone->getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
            this->_ardrone->configure("general:navdata_demo", "TRUE");
            break;
        }
        else
        {
            if (trytimes > 5)
            {
                this->print("Receive ardrone status TIMEOUT!");
                return A_FAILED;
            }
            trytimes++;
            this->_navSocket->writeDatagram(datagram, *(this->_ipAddr), NAV_DATA_PORT);
        }
    }

    /* Receive ARDrone status, wait for 1 seconds to timeout*/
    trytimes = 0;
    while(true)
    {
        if (this->_navSocket->waitForReadyRead(1000))
        {
            QByteArray rec_datagram;
            rec_datagram.resize(this->_navSocket->pendingDatagramSize());
            this->_navSocket->readDatagram(rec_datagram.data(), rec_datagram.size());

            if (!this->isNavPkt(rec_datagram))  // check it is a navdata datagram
                continue;

            uint state = this->getState(rec_datagram);

            /* ARDRONE_COMMAND_MASK is activated */
            if (state & ARDRONE_COMMAND_MASK && state & ARDRONE_NAVDATA_DEMO_MASK)
            {
                string cmd = "AT*CTRL=" + itos(this->_ardrone->getSeqNum()) + ",5,0";
                #ifdef DEBUG_NAVDATA_C
                this->print(cmd, this->_ardrone->sendAT_CMD(cmd););
				#else
				this->_ardrone->sendAT_CMD(cmd);
                #endif
                return A_OK;
            }
            else
            {
                if (trytimes > 5)
                {
                    this->print("Initialize failed after try 5 times");
                    return A_FAILED;
                }
                trytimes++;
				this->_ardrone->sendf("AT*CONFIG_IDS=%d,\"%s\",\"%s\",\"%s\"\r", _ardrone->getSeqNum(), ARDRONE_SESSION_ID, ARDRONE_PROFILE_ID, ARDRONE_APPLOCATION_ID);
                this->_ardrone->configure("general:navdata_demo", "TRUE");
            }
        }
        else
        {
            this->print("Receive ardrone status TIMEOUT!");
            return A_FAILED;
        }
    }
}

quint32 navData::getState(QByteArray byteArray)
{
    char * dat = byteArray.data();
    dat += NAVDATA_STATE_OFFSET;
    return *((quint32*)dat);
}

bool navData::isNavPkt(const QByteArray datagram)
{
    const char * dat = datagram.data();
    if (*((quint32*)dat) == NAVDATA_HEADER)
        return true;
    else
        return false;
}

RESULT navData::ardrone_navdata_unpack_all(navdata_unpacked_t *navdata_unpacked, navdata_t *navdata, quint32 *cks)
{

    RESULT res;
    navdata_cks_t navdata_cks;
    navdata_option_t* navdata_option_ptr;

    navdata_option_ptr = (navdata_option_t*) &(navdata->options[0]);
    memset( &navdata_cks, 0, sizeof(navdata_cks));
    memset( navdata_unpacked, 0, sizeof(navdata_unpacked_t) );

    navdata_unpacked->nd_seq          = navdata->sequence;
    navdata_unpacked->ardrone_state   = navdata->ardrone_state;
    navdata_unpacked->vision_defined  = navdata->vision_defined;

    res = A_OK;

    while( navdata_option_ptr != NULL )
    {
      // Check if we have a valid option
      if( 0 == navdata_option_ptr->size )
      {
          this->print("One option " + itos(navdata_option_ptr->tag) + " is not a valid option because its size is zero");
          navdata_option_ptr = NULL;
          res = A_FAILED;
      }
      else
      {
          if( navdata_option_ptr->tag <= NAVDATA_NUM_TAGS)
          {
               //#ifdef DEBUG_NAVDATA_C
               //if (navdata_unpacked->ardrone_state & ARDRONE_COMMAND_MASK)
               //{
               //    ack = "[ACK]";
               //}
               //this->print(ack + "Received navdatas tags :" + itos(navdata_option_ptr->tag));
               //ack = "";
               //#endif

               navdata_unpacked->last_navdata_refresh |= NAVDATA_OPTION_MASK(navdata_option_ptr->tag);
          }


         switch( navdata_option_ptr->tag )
         {

              #define NAVDATA_OPTION(STRUCTURE,NAME,TAG) \
              case TAG: \
              navdata_option_ptr = ardrone_navdata_unpack( navdata_option_ptr, navdata_unpacked->NAME); \
              break;

              #define NAVDATA_OPTION_DEMO(STRUCTURE,NAME,TAG)  NAVDATA_OPTION(STRUCTURE,NAME,TAG)
              #define NAVDATA_OPTION_CKS(STRUCTURE,NAME,TAG) {}

              #include "..//NAVDATA/navdata_keys.h"

          case NAVDATA_CKS_TAG:
              navdata_option_ptr = ardrone_navdata_unpack( navdata_option_ptr, navdata_cks );
              *cks = navdata_cks.cks;
              navdata_option_ptr = NULL; // End of structure
              break;


          default:
              this->print("Tag" + itos(navdata_option_ptr->tag) + "is an unknown navdata option tag");
              navdata_option_ptr = (navdata_option_t *)(((quint32)navdata_option_ptr) + navdata_option_ptr->size);
              break;
          }
       }
    }

    return res;

}

quint8 * navData::navdata_unpack_option(quint8 *navdata_ptr, quint32 ptrsize, quint8 *data, quint32 datasize)
{
    quint32 minSize = (ptrsize < datasize) ? ptrsize : datasize;
    memcpy(data, navdata_ptr, minSize);
    return (navdata_ptr + ptrsize);
}

quint32 navData::get_battery()
{
    return this->_navdata_unpacked.navdata_demo.vbat_flying_percentage;
}

float navData::get_pitch()
{
    return this->_navdata_unpacked.navdata_demo.theta;
}

float navData::get_roll()
{
    return this->_navdata_unpacked.navdata_demo.phi;
}

float navData::get_yaw()
{
    return this->_navdata_unpacked.navdata_demo.psi;
}

float navData::get_vx()
{
    return this->_navdata_unpacked.navdata_demo.vx;
}

float navData::get_vy()
{
    return this->_navdata_unpacked.navdata_demo.vy;
}

float navData::get_vz()
{
    return this->_navdata_unpacked.navdata_demo.vz;
}

qint32 navData::get_altitude()
{
    return this->_navdata_unpacked.navdata_demo.altitude;
}

quint32 navData::get_ctrl_state()
{
    return this->_navdata_unpacked.navdata_demo.ctrl_state;
}

bool navData::isEmergency()
{
    if (this->_navdata_unpacked.ardrone_state & ARDRONE_EMERGENCY_MASK)
        return true;
    else
        return false;
}

void navData::print(const string mesg)
{
    this->_ardrone->print(mesg);
}

void navData::print(const string mesg, const RESULT res)
{
    this->_ardrone->print(mesg, res);
}
void navData::print(const QString mesg)
{
    this->_ardrone->print(mesg);
}

void navData::print(const QString mesg, const RESULT res)
{
    this->_ardrone->print(mesg, res);
}

void navData::print(const char *mesg)
{
    this->_ardrone->print(mesg);
}

void navData::print(const char * mesg, const RESULT res)
{
    this->_ardrone->print(mesg, res);
}

void navData::navStart()
{
    this->go();
}

void navData::navStop()
{
	emit this->stopNavUpdate();
    this->stop();
}
