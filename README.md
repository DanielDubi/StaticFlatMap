# StaticFlatMap
A high performance map. 

This is a map implementation (actually a multimap) implemented with performance first in mind.
The map uses concurrent memory giving fast iteration and copy. Though resulting in slow inserts
and removes. 

The implementation is based on keepinag a sorted array as the map object and searching with
a binary search for lookups.

It's good when
  1. You can know your keys in advance
  2. You have a small map
  3. You have small objects
  
No so good when:
  1. The map is large
  2. The objects are large
  3. There are a lot of insertions and deletions
  
  
Good luck and have fun

Quick Installation:

System Wide:
```
mkdir build
cd build/
cmake ..
make
make install
```

Project:
```
add_subdirectory(StaticFlatMap)
target_link_libraries(<target> PUBLIC FlatMap)
```

OR

```
cp -R include/FlatMap <your-include-directory>
```
