#include "pch.h"

#include "tools.h"
#include "eg97park_radiotap_parser.h"


int main(int argc, char* argv[])
{
    Param param = {
        .if_ = nullptr,
        .mac_ = nullptr
    };

    if (!parse(&param, argc, argv))
    {
        return -1;
    }

    char errbuf[PCAP_ERRBUF_SIZE];
    pcap_t* handle = pcap_open_live(param.if_, BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL) {
        fprintf(stderr, "pcap_open_live(%s) return null - %s\n", param.if_, errbuf);
        return -1;
    }

    while (true)
    {
        sleep(0);
        struct pcap_pkthdr* header;
        const u_char* packet;
        int res = pcap_next_ex(handle, &header, &packet);
        if (res == 0)
        {
            continue;
        }

        if (res == PCAP_ERROR || res == PCAP_ERROR_BREAK)
        {
            printf("pcap_next_ex return %d(%s)\n", res, pcap_geterr(handle));
            break;
        }

        // Check beacon frame.
        dot11_radiotap_hdr* pkthdr_rtap = (dot11_radiotap_hdr*)packet;
        dot11_base_fhdr* pkthdr_frame_base = (dot11_base_fhdr*)(packet + pkthdr_rtap->it_len);
        if (pkthdr_frame_base->fctl_field != TYPE_BEACON_FRAME)
        {
            continue;
        }

        // Check MAC address.
        dot11_beacon_fhdr* pkthdr_frame_beacon = (dot11_beacon_fhdr*)(packet + pkthdr_rtap->it_len);
        if (std::memcmp(pkthdr_frame_beacon->src_addr, param.mac_, 6) != 0)
        {
            continue;
        }

        // Setup radiotap parser.
        RadiotapParser rtparser = RadiotapParser((uint8_t*)packet);
        if (rtparser.get_header_length() == 13){
            continue;
        }

        std::vector<uint32_t> presents_vector = rtparser.get_presents();
        std::map<dot11_relem_enum, uint32_t> rtap_map = rtparser.get_radiotap_data_map();

        int8_t antenna_signal = 0;
        std::map<dot11_relem_enum, uint32_t>::iterator is_exists_antsignal = rtap_map.find(IEEE80211_RADIOTAP_DBM_ANTSIGNAL);
        if (is_exists_antsignal != rtap_map.end())
        {
            antenna_signal = rtap_map.at(IEEE80211_RADIOTAP_DBM_ANTSIGNAL);
        }
        printf("[%s]: %ddbm\r", argv[2], antenna_signal);
    }
    
    
	return 0;
}
