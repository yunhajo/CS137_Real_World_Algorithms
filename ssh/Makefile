CC = clang
CFLAGS = -Iinclude -Wall -Wextra -O3 #-g -fsanitize=address,undefined

bin/keygen: out/rsa.o out/base64.o out/keygen.o out/rsa_private_key.o bin/libbigint.a
	$(CC) $(CFLAGS) $^ -o $@

bin/libbigint.a: out/bigint.o out/random.o
	ar -cr $@ $^

out/%.o: src-given/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

out/%.o: src/%.c
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	rm -f bin/*
	rm -f out/*
