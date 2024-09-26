from subprocess import run, Popen, PIPE
from sys import executable
import sys

cache = 	sys.argv[1]
binary = 	cache + "_cache"
test_dir = 	"tests/" + cache + "_tests/"
program = "./build/src/" + binary

num_test = 1
is_ok = True
for i in range(1, 11):
	str_data = test_dir
	
	str_data += str(i) + ".dat"
	file_in = open(str_data, "r")
	
	str_ans = test_dir
	str_ans += str(i) + ".dat.ans"

	ans = ""
	for i in open(str_ans):
		ans = i
	
	result = run(program, capture_output = True, encoding='cp866', stdin=file_in)
	print("Test: " + str(num_test).strip())
	print("Result: " + str(result.stdout).strip())
	
	is_ok &= (result.stdout == ans)
	if result.stdout == ans:
		print("OK")
	else:
		print("ERROR, correct: " + str(ans).strip())
	print("-------------------------------------------------")
	num_test += 1

if is_ok:
	print("TESTS PASSED")
else:
	print("TESTS FAILED")