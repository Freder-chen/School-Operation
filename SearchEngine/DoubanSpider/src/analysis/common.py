import json


def read_json_to_dict(jsonfile_path):
    with open(jsonfile_path, 'r', encoding='utf-8') as f:
        return json.loads(f.read())


def print_dict_to_json(dic, jsonfile_path):
    out = json.dumps(dic, ensure_ascii=False)
    with open(jsonfile_path, 'w', encoding='utf-8') as f:
        f.write(out)
