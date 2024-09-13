import random

TEST_NUM = 10

for i in range(1, TEST_NUM + 1):
    file_str = "lfu_tests/" + str(i) + ".dat"
    file = open(file_str, "w")

    cache_size  = random.randint(100, 200)
    request_num = random.randint(1000, 1500)
    test_str = str(cache_size) + " " + str(request_num) + "\n"

    for i in range(0, request_num):
        test_str += str(random.randint(1, 400)) + " "

    file.write(test_str)
    file.close()

    print(f"test data/{i}.dat generated")