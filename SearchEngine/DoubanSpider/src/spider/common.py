import json


# Some User Agents
hds=[
    {'User-Agent':'Mozilla/5.0 (Windows; U; Windows NT 6.1; en-US; rv:1.9.1.6) Gecko/20091201 Firefox/3.5.6'},\
    {'User-Agent':'Mozilla/5.0 (Windows NT 6.2) AppleWebKit/535.11 (KHTML, like Gecko) Chrome/17.0.963.12 Safari/535.11'},\
    {'User-Agent': 'Mozilla/5.0 (compatible; MSIE 10.0; Windows NT 6.2; Trident/6.0)'}
]


def print_dict_to_json(dic, jsonfile_path):
    out = json.dumps(dic, ensure_ascii=False)
    with open(jsonfile_path, 'w', encoding='utf-8') as f:
        f.write(out)