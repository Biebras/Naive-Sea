# Naive-Sea
This sea was created using the sum of sins. The method is a quick and straightforward technique for creating ocean simulations. However, it has a limitation: the sea may appear tiled when viewed from a distance. This issue can impact the realism of the simulation, particularly in scenarios where large-scale views are common.

https://github.com/Biebras/Naive-Sea/assets/49594342/709046a7-f330-44f2-8874-d5a715be0f92
## How to run
```
git clone --recurse-submodules https://github.com/Biebras/Naive-Sea.git
cd ./build/
cmake ..
make
./NaiveSea
```
## Resources
https://github.com/GarrettGunnell/Water \
https://developer.nvidia.com/gpugems/gpugems/part-i-natural-effects/chapter-1-effective-water-simulation-physical-models \
https://en.wikipedia.org/wiki/Snell%27s_law \
https://en.wikipedia.org/wiki/Schlick%27s_approximation
