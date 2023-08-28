import uvicorn
import apps

if __name__ == "__main__":
    uvicorn.run("apps:app", host="0.0.0.0", port=5000, reload=True)
