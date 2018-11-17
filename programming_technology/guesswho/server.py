from bottle import route, run, template, static_file, post, request, redirect
import sys

import question_generator

global question_generator_i
global level


def read_file(filename: str):
    try:
        file_content = open(filename, "r").read()
        return file_content
    except Exception:
        raise Exception("Can't read file " + filename)


def prepare_html(complexity: int):
    html = read_file("static/index.html")
    question = question_generator_i.generate_question(complexity)
    html = html.replace("{{IMAGE_URL}}", question[2])
    sel_data = "".join(
        ["<option value=" + str(i) + ">" + question[1][i] + "</option>" for i in range(0, len(question[1]) - 1)])
    html = html.replace("{{SELECT_DATA}}", sel_data)
    html = html.replace("{{QUESTION_ID}}", str(question[0]))
    return html


@route("/")
def index_request():
    redirect("/10/")
    level="/10/"
    return


@route("/<complexity>/")
def page_request(complexity):
    html = prepare_html(complexity)
    return html

@route("/change_complexity/")
def change_complexity():
    val = request.query['complexity']
    if (val is None):
        redirect(index_request)
    global level
    level="/"+val+"/"
    redirect("/"+val+"/")
    return


@post("/checkanswer")
def check_answer():
    question_id = int(request.forms.get("question_id"))
    answer = int(request.forms.get("selected_name"))
    res = question_generator_i.check_answer(question_id, answer)
    if res:
        return '<p>YES</p> <a href="http://localhost:8080' + level + '"> Еще раз </a>'
    else:
        return '<p>NO</p> <a href="http:\\\\localhost:8080'+ level + '"> Еще раз </a>'


def main(args):
    if len(args) < 2:
        print("First argument must be file name which contains names")
        return 1
    global question_generator_i
    global level
    level="/10/"
    question_generator_i = question_generator.QuestionGenerator(args[1], 5)
    run(host="localhost", port=8080)


if __name__ == '__main__':
    main(sys.argv)
