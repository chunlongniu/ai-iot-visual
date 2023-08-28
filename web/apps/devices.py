from fastapi import APIRouter, HTTPException

router = APIRouter(
    prefix="/devices",
    tags=["devices"],
    responses={404: {"description": "Not found"}}
)


@router.get("/")
async def list_all_devices():
    return ["all"]


@router.get("/{_id}")
async def get_device_by_id(_id: str):
    return _id


@router.post("/")
async def create_device():
    pass
