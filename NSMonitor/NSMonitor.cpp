// -*- C++ -*-
/*!
 * @file
 * @brief
 * @date
 * @author
 *
 */

#include "NSMonitor.h"

using DAQMW::FatalType::DATAPATH_DISCONNECTED;
using DAQMW::FatalType::INPORT_ERROR;
using DAQMW::FatalType::HEADER_DATA_MISMATCH;
using DAQMW::FatalType::FOOTER_DATA_MISMATCH;
using DAQMW::FatalType::USER_DEFINED_ERROR1;

// Module specification
// Change following items to suit your component's spec.
static const char* nsmonitor_spec[] =
{
    "implementation_id", "NSMonitor",
    "type_name",         "NSMonitor",
    "description",       "NSMonitor component",
    "version",           "1.0",
    "vendor",            "Hiroshi Sendai, KEK",
    "category",          "example",
    "activity_type",     "DataFlowComponent",
    "max_instance",      "1",
    "language",          "C++",
    "lang_type",         "compile",
    ""
};

NSMonitor::NSMonitor(RTC::Manager* manager)
    : DAQMW::DaqComponentBase(manager),
      m_InPort("nsmonitor_in",   m_in_data),
      m_in_status(BUF_SUCCESS),
      m_canvas(0),
      m_hist(0),
      m_bin(0),
      m_min(0),
      m_max(0),
      m_monitor_update_rate(30),
      m_event_byte_size(0),
      m_debug(false)
{
    // Registration: InPort/OutPort/Service

    // Set InPort buffers
    registerInPort ("nsmonitor_in",  m_InPort);

    init_command_port();
    init_state_table();
    set_comp_name("NSMONITOR");
}

NSMonitor::~NSMonitor()
{
}

RTC::ReturnCode_t NSMonitor::onInitialize()
{
    if (m_debug) {
        std::cerr << "NSMonitor::onInitialize()" << std::endl;
    }

    return RTC::RTC_OK;
}

RTC::ReturnCode_t NSMonitor::onExecute(RTC::UniqueId ec_id)
{
    daq_do();

    return RTC::RTC_OK;
}

int NSMonitor::daq_dummy()
{
    if (m_canvas) {
        m_canvas->Update();
        // daq_dummy() will be invoked again after 10 msec.
        // This sleep reduces X servers' load.
        sleep(1);
    }

    return 0;
}

int NSMonitor::daq_configure()
{
    std::cerr << "*** NSMonitor::configure" << std::endl;

    ::NVList* paramList;
    paramList = m_daq_service0.getCompParams();
    parse_params(paramList);

    return 0;
}

int NSMonitor::parse_params(::NVList* list)
{

    std::cerr << "param list length:" << (*list).length() << std::endl;

    int len = (*list).length();
    for (int i = 0; i < len; i+=2) {
        std::string sname  = (std::string)(*list)[i].value;
        std::string svalue = (std::string)(*list)[i+1].value;

        std::cerr << "sname: " << sname << "  ";
        std::cerr << "value: " << svalue << std::endl;
        
        if (sname == "monitorUpdateRate") {
            if (m_debug) {
                std::cerr << "monitor update rate: " << svalue << std::endl;
            }
            char *offset;
            m_monitor_update_rate = (int)strtol(svalue.c_str(), &offset, 10);
        }
        // If you have more param in config.xml, write here
    }

    return 0;
}

int NSMonitor::daq_unconfigure()
{
    std::cerr << "*** NSMonitor::unconfigure" << std::endl;
    if (m_canvas) {
        delete m_canvas;
        m_canvas = 0;
    }

    if (m_hist) {
        delete m_hist;
        m_hist = 0;
    }
    return 0;
}

int NSMonitor::daq_start()
{
    std::cerr << "*** NSMonitor::start" << std::endl;

    m_in_status  = BUF_SUCCESS;

    //////////////// CANVAS FOR HISTOS ///////////////////
    if (m_canvas) {
        delete m_canvas;
        m_canvas = 0;
    }
    m_canvas = new TCanvas("c1", "histos", 0, 0, 600, 400);

    ////////////////       HISTOS      ///////////////////
    if (m_hist) {
        delete m_hist;
        m_hist = 0;
    }

    //int m_hist_bin = 100;
    int    m_hist_bin = 4096;
    double m_hist_min = 0.0;
    double m_hist_max = 4096.0;

    gStyle->SetStatW(0.4);
    gStyle->SetStatH(0.2);
    gStyle->SetOptStat("em");

    m_hist = new TH1D("hist", "hist", m_hist_bin, m_hist_min, m_hist_max);
    m_hist->GetXaxis()->SetNdivisions(5);
    m_hist->GetYaxis()->SetNdivisions(4);
    m_hist->GetXaxis()->SetLabelSize(0.07);
    m_hist->GetYaxis()->SetLabelSize(0.06);

    return 0;
}

int NSMonitor::daq_stop()
{
    std::cerr << "*** NSMonitor::stop" << std::endl;

    m_hist->Draw();
    m_canvas->Update();

    reset_InPort();

    return 0;
}

int NSMonitor::daq_pause()
{
    std::cerr << "*** NSMonitor::pause" << std::endl;

    return 0;
}

int NSMonitor::daq_resume()
{
    std::cerr << "*** NSMonitor::resume" << std::endl;

    return 0;
}

int NSMonitor::reset_InPort()
{
    int ret = true;
    while(ret == true) {
        ret = m_InPort.read();
    }

    return 0;
}

int NSMonitor::fill_data(const unsigned char* mydata, const int size)
{
    int n_data = size / ONE_DATA_BYTE_SIZE;
    for (int i = 0; i < n_data; i++) {
        // i: data index in this buffer.  numbered as data count
        // We don't implement separate decode function
        // because data format is very simple.
        // If we have more complicated format, we have to implement
        // decode function

        // __attribute__((unused)): suppress "unused variable warning when compile
        // count and flag are not used for now
        // unsigned char __attribute__((unused)) count = (mydata[0+2*i] & 0x7F);
        // unsigned char __attribute__((unused)) flag  = ((mydata[0+2*i] & 0x80) >> 7);
        // unsigned char data = mydata[1+i*2];
        m_NSdata.flag = (mydata[ONE_DATA_BYTE_SIZE*i + 0] & 0x80) >> 7;

        unsigned short *count_p;
        count_p = (unsigned short *)&mydata[ONE_DATA_BYTE_SIZE*i + 0];
        m_NSdata.count = ntohs(*count_p) & 0x7FFF; // drop flag bit

        unsigned short *data_p;
        data_p = (unsigned short *)&mydata[ONE_DATA_BYTE_SIZE*i + 2];
        m_NSdata.data = ntohs(*data_p);
        // XXX: very simple data check
        if (m_NSdata.data >= 4096) {
            std::cerr << "Invalid data: " << m_NSdata.data << std::endl;
        }

        m_hist->Fill(m_NSdata.data);
    }

    return 0;
}

unsigned int NSMonitor::read_InPort()
{
    /////////////// read data from InPort Buffer ///////////////
    unsigned int recv_byte_size = 0;
    bool ret = m_InPort.read();

    //////////////////// check read status /////////////////////
    if (ret == false) { // false: TIMEOUT or FATAL
        m_in_status = check_inPort_status(m_InPort);
        if (m_in_status == BUF_TIMEOUT) { // Buffer empty.
            if (check_trans_lock()) {     // Check if stop command has come.
                set_trans_unlock();       // Transit to CONFIGURE state.
            }
        }
        else if (m_in_status == BUF_FATAL) { // Fatal error
            fatal_error_report(INPORT_ERROR);
        }
    }
    else {
        recv_byte_size = m_in_data.data.length();
    }

    if (m_debug) {
        std::cerr << "m_in_data.data.length():" << recv_byte_size
                  << std::endl;
    }

    return recv_byte_size;
}

int NSMonitor::daq_run()
{
    if (m_debug) {
        std::cerr << "*** NSMonitor::run" << std::endl;
    }

    unsigned int recv_byte_size = read_InPort();
    if (recv_byte_size == 0) { // Timeout
        return 0;
    }

    check_header_footer(m_in_data, recv_byte_size); // check header and footer
    m_event_byte_size = get_event_size(recv_byte_size);
    if (m_event_byte_size > DATA_BUF_SIZE) {
        fatal_error_report(USER_DEFINED_ERROR1, "DATA BUF TOO SHORT");
    }

    /////////////  Write component main logic here. /////////////
    memcpy(&m_recv_data[0], &m_in_data.data[HEADER_BYTE_SIZE], m_event_byte_size);

    fill_data(&m_recv_data[0], m_event_byte_size);

    if (m_monitor_update_rate == 0) {
        m_monitor_update_rate = 1000;
    }

    unsigned long sequence_num = get_sequence_num();
    if ((sequence_num % m_monitor_update_rate) == 0) {
        m_hist->Draw();
        m_canvas->Update();
        std::cerr << "m_event_byte_size: " << m_event_byte_size << std::endl;
    }
    /////////////////////////////////////////////////////////////
    inc_sequence_num();                      // increase sequence num.
    inc_total_data_size(m_event_byte_size);  // increase total data byte size

    return 0;
}

extern "C"
{
    void NSMonitorInit(RTC::Manager* manager)
    {
        RTC::Properties profile(nsmonitor_spec);
        manager->registerFactory(profile,
                    RTC::Create<NSMonitor>,
                    RTC::Delete<NSMonitor>);
    }
};
