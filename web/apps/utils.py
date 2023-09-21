import yaml
import asyncio_mqtt as aiomqtt


def read_config(cfg="apps/config.yaml"):
    data = {}
    with open(cfg, 'r') as fd:
        data = yaml.safe_load(fd)
    return data


_config = read_config()


async def publish_msg(topic: str, msg: str):
    mqtt_cfg = _config.get("mqtt")
    async with aiomqtt.Client(
        hostname=mqtt_cfg.get("address"),
        port=mqtt_cfg.get("port"),
    ) as client:
        await client.publish(topic, payload=msg)
