from fastapi import FastAPI
from fastapi.staticfiles import StaticFiles

from . import devices

app = FastAPI()

app.mount("/html", StaticFiles(directory="html", html=True), name="html")


@app.get("/")
async def root():
    return {"message": "HelloWorld"}

app.include_router(devices.router)
