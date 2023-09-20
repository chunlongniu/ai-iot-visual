import logging
from fastapi import APIRouter, HTTPException

router = APIRouter(
    prefix="/devices",
    tags=["devices"],
    responses={404: {"description": "Not found"}}
)

logger = logging.getLogger(__name__)


@router.get("/")
async def list_all_devices():
    logging.info("list devices")
    return ["all"]


@router.get("/{_id}")
async def get_device_by_id(_id: str):
    return _id


@router.post("/")
async def create_device():
    return "this is a post"


@router.put("/{opera}")
async def update_device(opera: str):
    logging.info(f"the operation: {opera}")
    return f"The operation: {opera}"
