FROM gcc:latest
WORKDIR /usr/src/myapp
COPY analyzer.c .
COPY sizes.txt .
RUN gcc -o analyzer analyzer.c
CMD ["./analyzer"]

