#ifndef OUTPUT_GAZETTEER_H
#define OUTPUT_GAZETTEER_H

#include "output.hpp"

struct output_gazetteer_t : public output_t {
    output_gazetteer_t();
    virtual ~output_gazetteer_t();

    int start(const struct output_options *options);
    int connect(const struct output_options *options, int startTransaction);
    void stop();
    void cleanup(void);
    void close(int stopTransaction);

    int node_add(osmid_t id, double lat, double lon, struct keyval *tags);
    int way_add(osmid_t id, osmid_t *nodes, int node_count, struct keyval *tags);
    int relation_add(osmid_t id, struct member *members, int member_count, struct keyval *tags);

    int node_modify(osmid_t id, double lat, double lon, struct keyval *tags);
    int way_modify(osmid_t id, osmid_t *nodes, int node_count, struct keyval *tags);
    int relation_modify(osmid_t id, struct member *members, int member_count, struct keyval *tags);

    int node_delete(osmid_t id);
    int way_delete(osmid_t id);
    int relation_delete(osmid_t id);

private:
    static const size_t BUFFER_SIZE = 4096;

    void require_slim_mode(void);
    void copy_data(const char *sql);
    void stop_copy(void);
    void delete_unused_classes(char osm_type, osmid_t osm_id, struct keyval *places);
    void add_place(char osm_type, osmid_t osm_id, const char *key_class, const char *type,
                   struct keyval *names, struct keyval *extratags, int adminlevel, 
                   struct keyval *housenumber, struct keyval *street, struct keyval *addr_place, 
                   const char *isin, struct keyval *postcode, struct keyval *countrycode,
                   const char *wkt);
    void delete_place(char osm_type, osmid_t osm_id);
    int gazetteer_process_node(osmid_t id, double lat, double lon, struct keyval *tags,
                               int delete_old);
    int gazetteer_process_way(osmid_t id, osmid_t *ndv, int ndc, struct keyval *tags,
                              int delete_old);
    int gazetteer_process_relation(osmid_t id, struct member *members, int member_count,
                                   struct keyval *tags, int delete_old);

    const struct output_options *Options;

    struct pg_conn *Connection;
    struct pg_conn *ConnectionDelete;
    struct pg_conn *ConnectionError;
    
    int CopyActive;
    unsigned int BufferLen;
    
    FILE * hLog;
    
    slim_middle_t *slim_mid;

    char Buffer[BUFFER_SIZE];
};

extern output_gazetteer_t out_gazetteer;

#endif