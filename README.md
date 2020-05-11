# grid

## 概要
- ベクトル場・スカラー場の座標を量子化したmapを提供するライブラリ

## 使い方

- 多次元配列にアクセスする要領で、その座標が含まれる領域の値の参照を得ることが出来る。
- 宣言時に、その範囲を閉区間[min, max]として渡す。座標はこの閉区間外のときout of rangeとなる。

```cpp
#include <grid/core.hpp>

using Temperature = double;

int main()
{
    {
        // 固定分割
        Grid::GridArray<Temperature, double, 100, 100, 100> map{{-1.0, 1.0}, {-1.0, 1.0}, {-1.0, 1.0}};

        map.at(0.3, 0.02, 0.9) = 44.3;
        map[0.3][-0.1][-0.4] = 2.1;
    }

    {
        // 動的分割
        Grid::GridVector<Temperature, double, 3> map{{-1.0, 1.0, 100}, {-1.0, 1.0, 100}, {-1.0, 1.0, 100}};
    
        map.at(0.3, 0.02, 0.9) = 44.3;
        map[0.3][-0.1][-0.4] = 2.1;
    }

    return 0;
}
```

- for文でアクセスするのに便利なlinspace, arangeが用意されている。使い方はnumpyのそれと同じ。

```cpp
#include <grid/core.hpp>
#include <grid/linear.hpp>

using Temperature = double;

int main()
{
    Grid::GridArray<double, double, 100, 100> map{{-1.0, 1.0}, {-1.0, 1.0}};

    for( double& x : Grid::linspace(-1.0, 1.0, 100) ) {
        for( double& y : Grid::linspace(-1.0, 1.0, 100) ) {
            map[x][y] = x + y;
        }
    }

    // 量子化の計算のぶん、一時オブジェクトを保存したほうが高速
    for( double& x : Grid::linspace(-1.0, 1.0, 100) ) {
        auto tmp = map[x];
        for( double& y : Grid::linspace(-1.0, 1.0, 100) ) {
            std::cout << x << ' ' << y << ' ' << tmp[y] << std::endl;
        }
    }

    return 0;
}
```

- このライブラリの実装に必要だった多次元のarray, vectorも利用可能
- データはメモリ上1次元に並ぶ。

```cpp
#include <grid/array.hpp>

using Temperature = double;

int main()
{
    Grid::array<double, 100, 100> ar{};

    ar.fill(0.0);

    ar[21][11];

    return 0;
}
```


