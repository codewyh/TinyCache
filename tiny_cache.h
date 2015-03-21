
#include <string>
#include <unordered_map>
#include <map>
#include <set>
#include <inttypes.h>

struct TinyCacheValue 
{
	int64_t timestamp;
	std::string data;

	TinyCacheValue(int64_t time, std::string _data) : timestamp(time), data(_data)
	{

	}

	TinyCacheValue() = default;
};

class TinyCache
{
public:
	TinyCache(){}
	std::string GetCache(std::string& key);
	void PutCache(std::string& key, std::string& value, int64_t expired_time_span);

	int64_t GetCacheExpireTime(std::string& key);
	void Erase(std::string& key);
	void Update();

protected:
private:
	std::unordered_map<std::string, TinyCacheValue> kv_map_;

	std::map<int64_t, std::set<std::string>> map_of_timer_key_;
};

