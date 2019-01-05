#-*- coding: UTF-8 -*-

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.patches import Polygon
from mpl_toolkits.basemap import Basemap
from matplotlib.font_manager import FontProperties

# mac 使用
font = FontProperties(fname='/Library/Fonts/Songti.ttc',size=7)
# win 使用
# plt.rcParams['font.sans-serif']=['SimHei'] #用来正常显示中文标签
# plt.rcParams['axes.unicode_minus']=False #用来正常显示负号

color_count = 4

# provinces_name = [
#     '青海', '广西壮族自治区', '重庆', '山东', '甘肃', '安徽', '广东', '陕西', '黑龙江省', 
#     '西藏自治区', '云南', '湖南', '江苏', '吉林', '山西', '宁夏回族自治区', '河北', '浙江', 
#     '四川', '新疆维吾尔自治区', '海南', '天津', '贵州', '内蒙古自治区', '河南', '福建', '北京', 
#     '江西', '上海', '辽宁', '湖北'
# ]


def get_provinces_adjacent():
    provinces_adjacent = {}
    provinces_adjacent['海南'] = ['广东']
    provinces_adjacent['天津'] = ['北京', '河北']
    provinces_adjacent['北京'] = ['天津', '河北']
    provinces_adjacent['上海'] = ['江苏', '浙江']
    provinces_adjacent['福建'] = ['浙江', '江西', '广东']
    provinces_adjacent['辽宁'] = ['吉林', '河北', '内蒙古自治区']
    provinces_adjacent['山东'] = ['河北', '河南', '安徽', '江苏']
    provinces_adjacent['江苏'] = ['山东', '安徽', '浙江', '上海']
    provinces_adjacent['吉林'] = ['辽宁', '黑龙江省', '内蒙古自治区']
    provinces_adjacent['黑龙江省'] = ['吉林', '辽宁', '内蒙古自治区']
    provinces_adjacent['山西'] = ['河北', '陕西', '河南', '内蒙古自治区']
    provinces_adjacent['浙江'] = ['福建', '江西', '安徽', '上海', '江苏']
    provinces_adjacent['重庆'] = ['湖南', '湖北', '四川', '贵州', '陕西']
    provinces_adjacent['宁夏回族自治区'] = ['陕西', '甘肃', '内蒙古自治区']
    provinces_adjacent['新疆维吾尔自治区'] = ['甘肃', '青海', '西藏自治区']
    provinces_adjacent['广西壮族自治区'] = ['广东', '湖南', '贵州', '云南']
    provinces_adjacent['云南'] = ['四川', '贵州', '广西壮族自治区', '西藏自治区']
    provinces_adjacent['湖北'] = ['河南', '安徽', '重庆', '江西', '湖南', '陕西']
    provinces_adjacent['江西'] = ['浙江', '福建', '广东', '湖南', '湖北', '安徽']
    provinces_adjacent['安徽'] = ['山东', '江苏', '浙江', '江西', '湖北', '河南']
    provinces_adjacent['西藏自治区'] = ['云南', '四川', '青海', '新疆维吾尔自治区']
    provinces_adjacent['贵州'] = ['四川', '重庆', '湖南', '云南', '广西壮族自治区']
    provinces_adjacent['四川'] = ['青海', '云南', '贵州', '重庆', '陕西', '西藏自治区']
    provinces_adjacent['河南'] = ['河北', '山西', '陕西', '湖北', '安徽', '江苏', '山东']
    provinces_adjacent['广东'] = ['海南', '福建', '江西', '湖南', '海南', '广西壮族自治区']
    provinces_adjacent['湖南'] = ['湖北', '广东', '重庆', '贵州', '江西', '广西壮族自治区']
    provinces_adjacent['河北'] = ['辽宁', '山西', '河南', '山东', '北京', '天津', '内蒙古自治区']
    provinces_adjacent['青海'] = ['四川', '陕西', '甘肃', '西藏自治区', '宁夏回族自治区', '新疆维吾尔自治区']
    provinces_adjacent['甘肃'] = ['四川', '青海', '陕西', '内蒙古自治区', '宁夏回族自治区', '新疆维吾尔自治区']
    provinces_adjacent['陕西'] = ['四川', '重庆', '湖北', '河南', '山西', '甘肃', '内蒙古自治区', '宁夏回族自治区']
    provinces_adjacent['内蒙古自治区'] = ['甘肃', '陕西', '山西', '河北', '辽宁', '吉林', '黑龙江省','宁夏回族自治区']
    return provinces_adjacent


def get_provinces_xy():
    provinces_name_xy = {
        '安徽': [4285767, 1892022],
        '重庆': [3410000, 1550019],
        '海南': [3760000, 350000],
        '广东': [4120000, 860289],
        '云南': [2850000, 897868],
        '北京': [4049593, 2777977],
        '福建': [4500000, 1220000],
        '甘肃': [2320000, 2669013],
        '贵州': [3390000, 1200000],
        '河北': [4010000, 2600000],
        '河南': [3900000, 2064736],
        '湖北': [3822160, 1700873],
        '湖南': [3822160, 1328225],
        '江苏': [4500000, 2050321],
        '江西': [4210000, 1390000],
        '吉林': [4772203, 3310000],
        '辽宁': [4531000, 3040000],
        '陕西': [3510000, 2102636],
        '山东': [4280000, 2380000],
        '上海': [4720000, 1882022],
        '山西': [3740000, 2435000],
        '天津': [4150000, 2669013],
        '浙江': [4630000, 1621163],
        '四川': [2936536, 1550000],
        '青海': [2369370, 2129154],
        '黑龙江省': [4716660, 3759137],
        '西藏自治区': [1615770, 1691560],
        '内蒙古自治区': [3334719, 2702431],
        '广西壮族自治区': [3605176, 828289],
        '宁夏回族自治区': [3212518, 2319222],
        '新疆维吾尔自治区': [1457225, 2754141],
    }
    return provinces_name_xy


def init_map_node(provinces_adjacent):
    map_color_dict = {}
    for key in provinces_adjacent.keys():
        map_color_dict[key] = 0
    return map_color_dict


def is_ok(provinces_adjacent, map_color_dict, node):
    colors = [map_color_dict[n] for n in provinces_adjacent[node]]
    if map_color_dict[node] in colors:
        return False
    else:
        return True


def color_map(provinces_adjacent, map_color_dict):
    '''
        DFS
    '''
    global color_count

    map_color_dict = map_color_dict.copy()

    if 0 not in list(map_color_dict.values()):
        return map_color_dict
    else:
        for key, value in map_color_dict.items():
            if value == 0:
                node = key
                break

        for color in range(1, color_count + 1):
            map_color_dict[node] = color
            if is_ok(provinces_adjacent, map_color_dict, node):
                mcd = color_map(provinces_adjacent, map_color_dict)
                if mcd != None: return mcd
        return None


def get_china_map():
    m = Basemap(
        llcrnrlon=77,
        llcrnrlat=14,
        urcrnrlon=140,
        urcrnrlat=51,
        projection='lcc',
        lat_1=33,
        lat_2=45,
        lon_0=100
    )
    m.readshapefile('./gadm36_CHN_shp/gadm36_CHN_1', 'states', drawbounds=True)
    return m


def print_china_map(map_color_dict):
    plt.figure(figsize=(10, 7))

    colors = ['white', 'red', 'blue', 'green', 'yellow', 'black']
    china_map = get_china_map()

    # fill color
    ax = plt.gca()
    for index, info in enumerate(china_map.states_info):
        p = info['NL_NAME_1'].split('|')
        s = p[1] if len(p) > 1 else p[0]
        if s == '黑龍江省': s = '黑龙江省'
        seg = china_map.states[index]
        color = colors[map_color_dict[s]]
        poly = Polygon(seg, facecolor=color)
        ax.add_patch(poly)

    # fill title
    provinces_name_xy = get_provinces_xy()
    for key, value in provinces_name_xy.items():
        ax.text(value[0], value[1], key, horizontalalignment='center', verticalalignment='center', fontproperties=font)

    plt.show()


def main():
    provinces_adjacent = get_provinces_adjacent()
    map_color_dict = init_map_node(provinces_adjacent)
    provinces_name = list(provinces_adjacent.keys())
    
    map_color_dict = color_map(provinces_adjacent, map_color_dict)
    print(map_color_dict)

    print_china_map(map_color_dict)


if __name__ == '__main__':
    main()
