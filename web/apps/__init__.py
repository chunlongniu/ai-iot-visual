import logging
from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

from . import devices

app = FastAPI()
app.mount("/html", StaticFiles(directory="html", html=True), name="html")


@app.get("/")
async def root():
    logging.info("root request")
    return {"message": "HelloWorld -- "}

app.include_router(devices.router)
