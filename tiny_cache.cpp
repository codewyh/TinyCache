#include <algorithm>
#include <time.h>
#include "tiny_cache.h"


std::string TinyCache::GetCache(std::string& key)
{
	auto it = kv_map_.find(key);
	std::string value = (it != kv_map_.end()) ? it->second.data : std::string();
	return value;
}

void TinyCache::PutCache(std::string& key, std::string& value, int64_t expired_time_span)
{
  	int64_t now = time(0);
	int64_t expired_time = now + expired_time_span;

	auto kv_it = kv_map_.find(key);
	if (kv_it != kv_map_.end())
	{
		auto time_it = map_of_timer_key_.find(kv_it->second.timestamp);
		if (time_it != map_of_timer_key_.end())
		{
			time_it->second.erase(key);
			if (time_it->second.empty())
			{
				map_of_timer_key_.erase(time_it);
			}
		}
	}
	if (expired_time_span != 0)
	{
		std::set<std::string>& key_set = map_of_timer_key_[expired_time];
		key_set.insert(key);
	}
	else
	{
		expired_time = 0;
	}
	kv_map_[key] = TinyCacheValue(expired_time, value);
}

int64_t TinyCache::GetCacheExpireTime(std::string& key)
{
	auto it = kv_map_.find(key);
	if (it != kv_map_.end())
	{
		return it->second.timestamp;
	}
	return 0;
}

void TinyCache::Update()
{
  	int64_t now = time(0);

	std::vector<int64_t> erase_vec;
	for (auto it : map_of_timer_key_)
	{
		if (now < it.first)
		{
			break;
		}

		for (auto key : it.second)
		{
			auto kv_it = kv_map_.find(key);
			std::string _value;
			if (kv_it != kv_map_.end())
			{
				_value = kv_it->second.data;
			}
			kv_map_.erase(key);
			erase_vec.push_back(it.first);
		}
	}

	for_each(erase_vec.begin(), erase_vec.end(), [&](int64_t time){ map_of_timer_key_.erase(time); });
}

void TinyCache::Erase(std::string& key)
{
	auto it = kv_map_.find(key);
	if (it != kv_map_.end())
	{
		map_of_timer_key_.erase(it->second.timestamp);
		kv_map_.erase(it);
	}
}
