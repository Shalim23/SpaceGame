from abc import ABC, abstractmethod

class DataTypeBase(ABC):

    @abstractmethod
    def binarize(self) -> bytes:
        pass

    @abstractmethod
    def get_data_type_name(self) -> str:
        pass
