


#define NGX_HTTP_CACHE_ENTRY_DELETED  0x00000001
#define NGX_HTTP_CACHE_ENTRY_MMAPED   0x00000002

/* "/" -> "/index.html" in ngx_http_index_handler */
#define NGX_HTTP_CACHE_ENTRY_URI      0x00000004

#define NGX_HTTP_CACHE_FILTER_FLAGS   0xFFFF0000


typedef struct {
    ngx_fd_t   fd;
    off_t      size;
    void      *data;
    time_t     accessed;
    time_t     last_modified;
    time_t     updated;      /* no needed with kqueue */
    int        refs;
    int        flags;
} ngx_http_cache_entry_t;


typedef struct {
    u_int32_t          crc;
    ngx_str_t          uri;
    ngx_http_cache_t  *cache;
} ngx_http_cache_hash_entry_t;


typedef struct {
    ngx_http_cache_t  *cache;
    u_int32_t          crc;
    int                n;
} ngx_http_cache_handle_t; 


int ngx_http_cache_get(ngx_http_cache_hash_t *cache_hash,
                       ngx_str_t *uri, ngx_http_cache_handle_t *h)
{
    int                           hi;
    ngx_http_cache_hash_entry_t  *entry;

    h->crc = ngx_crc32(uri->data, uri->len);

    hi = h->crc % cache_hash->size;
    entry = cache_hash[hi].elts;

    for (i = 0; i < cache_hash[hi].nelts; i++) {
        if (entry[i].crc == crc
            && entry[i].uri.len == uri->len
            && ngx_strncmp(entry[i].uri.data, uri->data, uri->len) == 0
        {
            h->cache = entry[i].cache;
            h->cache->refs++;
            h->n = hi;
            return NGX_OK;
        }
    }

    return NGX_ERROR;
}
