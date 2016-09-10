import os
import sys
from subprocess import Popen, PIPE, STDOUT
from collections import OrderedDict

ver = sys.version_info[0]

def get_config(argv):
    for i in range(len(argv) - 1):
        if argv[i] == "-c":
            return argv[i+1]
    return "spcbir.config"

def get_config_param(config, param, default = ""):
    f = open(config)

    value = default
    
    for line in f.readlines():
        line = line.strip()
        if line == "#" or line == "":
            continue

        if line.find("=") >= 0:
            if line.startswith(param):
                value = line.split("=")[1].strip()
    f.close()

    return value


def open_exe(argv):
    exe_list = argv[1:]
    return Popen(exe_list, stdout=PIPE, stderr=STDOUT, stdin=PIPE)


def send_input(proc, config):
    image_dir = get_config_param(config, "spImagesDirectory")
    assert(image_dir != "")

    image_prefix = get_config_param(config, "spImagesPrefix")
    assert(image_prefix != "")

    image_suffix = get_config_param(config, "spImagesSuffix")
    assert(image_suffix != "")
    
    numOfImages_str = get_config_param(config, "spNumOfImages")
    assert(numOfImages_str != "")

    numOfImages = int(numOfImages_str)
    for i in range(numOfImages):
        line = image_dir + image_prefix + str(i) + image_suffix + '\n'
        if ver < 3:
            proc.stdin.write(line)
        else:
            proc.stdin.write(bytes(line, 'UTF-8'))

    if ver < 3:
        proc.stdin.write('<>\n')
    else:
        proc.stdin.write(bytes('<>', 'UTF-8'))


def output_to_dict(out):
    if ver >= 3:
        out =out.decode('UTF-8')
    
    lines = out.replace("\r\n", "\n").split("\n")
    query = ""
    closest = []

    results = OrderedDict()
    for i, line in enumerate(lines):
        if line.startswith("Best"):
            query = line.split("-")[1].strip()
        elif line.startswith("Please") and query != "":
            results[query] = closest
            query = ""
            closest = []
        elif query != "":
            closest.append(line)

    return results


def dict_to_html(config, results):
    num_of_similar_images = int(get_config_param(config, "spNumOfSimilarImages", "1"))
    html = """
<html>
    <head>
    <title>Software Project - Final Project - Results</title>
    <style>
body {
    text-align: center;
}

table {
    border 0px;
    margin: 10px auto;
    padding: 0px;

    border-spacing: 0px;
    border-collapse: collapse;
}

tr {
    margin: 0px;
    padding: 0px;
}

td {
    border: 1px solid black;
    padding: 5px;
    text-align: center;
    width: 80px;
}

td.query, th.query {
    background-color: #fafafa;
    border-right: 3px solid black !important;
}

figure {
    margin: 0px;
}

img {
    height: 70px;
}
    </style>
    </head>
    <body>\n"""
    
    html += '''
        <table>
        <tr>
            <th class="query">query</td>\n'''
    for i in range(num_of_similar_images):
        html += '\t\t\t<th>no. ' + str(i+1) + '</th>'
    html += '''\t\t</tr>\n'''
    for query in results:
        closest = results[query]
        html += '''
        <tr>
            <td class="query" style="width: 70px;">
                <figure>
                    <img src="''' + query + '''" title="''' + query + '''" />
                    <figcaption>''' + query + '''</figcaption>
                </figure>
            </td>\n'''

        for image in closest:
            html += '''
            <td>
                <figure>
                    <img src="''' + image + '''" title="''' + image + '''" />
                    <figcaption>''' + image + '''</figcaption>
                </figure>
            </td>\n'''
        
        html += "\t\t</tr>\n"
    html += "\t\t</table>\n"
    
    html += """
    </body>
</html>"""

    return html


def main(argv):
    proc = open_exe(argv);

    config = get_config(argv)
    print "inserting inputs into program"
    send_input(proc, config)
    
    print "creating a image dictionary out of program's output"
    results = output_to_dict(proc.communicate()[0])
    
    print "generating HTML file"
    html = dict_to_html(config, results)
    f = open(os.path.splitext(config)[0] + ".html", "w")
    f.write(html)
    f.close()


if __name__ == "__main__":
    if len(sys.argv) > 1:
        args = sys.argv
    else:
        if ver < 3:
            command = raw_input("Enter Command line: ").split(" ")
        else:
            command = input("Enter Command line: ").strip().split(" ")
        args = sys.argv + command
    print("starting...")
    main(args)
    print("done!")
