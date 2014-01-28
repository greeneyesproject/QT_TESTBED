#ifndef RADIO_SYSTEM_H
#define RADIO_SYSTEM_H

enum{
  TABLE_SIZE = 2,
};

typedef nx_struct greeneyes_header_t {
  nx_am_addr_t src_addr;
  nx_am_addr_t dst_addr;
  nx_uint8_t packet_id;
  nx_uint16_t num_frames;
  nx_uint16_t frame_number;
  nx_uint8_t packet_info;

} greeneyes_header_t;

typedef struct routing_table_entry_t {
  am_addr_t dst_addr;
  am_addr_t next_hop;
} routing_table_entry_t;

#endif