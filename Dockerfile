FROM python:3.9-slim

WORKDIR /

COPY . /

RUN chmod +x /main.exe /execute.sh

ENTRYPOINT [ "bash", "execute.sh" ]
