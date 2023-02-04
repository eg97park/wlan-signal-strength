#include "pch.h"
#include "eg97park_wlan.h"

/**
 * @brief Radiotap Header 파싱용 클래스.
 * 
 */
class RadiotapParser
{
private:
    uint8_t* pkt_addr;
    uint8_t hdr_ver;
    uint8_t hdr_pad;
    uint16_t hdr_len;
    uint8_t* hdr_pst_addr;
    uint32_t* presents;
    std::vector<uint32_t> rtap_present_vector;
    std::map<dot11_relem_enum, uint32_t> rtap_data_map;
public:
    /**
     * @brief 생성자.
     * 
     * @param _radiotap_header_addr Radiotap Header 시작 주소. (패킷의 시작 주소와 동일)
     */
    RadiotapParser(uint8_t* _radiotap_header_addr);


    /**
     * @brief 소멸자.
     * 
     */
    ~RadiotapParser();

    
    /**
     * @brief Radiotap Header의 revision을 반환.
     * 
     * @return uint8_t 
     */
    uint8_t get_header_version();
    

    /**
     * @brief Radiotap Header의 padding을 반환.
     * 
     * @return uint8_t 
     */
    uint8_t get_header_padding();
    

    /**
     * @brief Radiotap Header의 length를 반환.
     * 
     * @return uint16_t 
     */
    uint16_t get_header_length();


    /**
     * @brief Radiotap Header의 첫 번째 present 값을 반환.
     * 
     * @return uint32_t 
     */
    uint32_t get_first_present();


    /**
     * @brief Radiotap Header의 모든 present들을 순서대로 정리한 vector의 형태로 반환.
     * 
     * @return std::vector<uint32_t> 
     */
    std::vector<uint32_t> get_presents();


    /**
     * @brief 현재 Radiotap Header에서 가지고 있는 Present Flag의 이름과 값을 map의 형태로 반환.
     *  사용법 예시: uint32_t radiotap_channel_value = get_radiotap_data_map().at(IEEE80211_RADIOTAP_CHANNEL);
     * 
     * @return std::map<dot11_relem_enum, uint32_t> 
     */
    std::map<dot11_relem_enum, uint32_t> get_radiotap_data_map();
};
